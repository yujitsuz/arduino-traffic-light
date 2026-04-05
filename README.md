# Arduino Two-Way Traffic Light System

A two-way traffic light controller built with Arduino, featuring a bidirectional pedestrian button. The system manages two intersecting traffic lights in real time using a non-blocking state machine — no `delay()` calls anywhere in the code.

---

## How it works

The core idea is a state machine driven by a single `color` variable that holds the current phase of the main traffic light (`'g'`, `'y'`, or `'r'`). The second traffic light always mirrors the opposite state — when the main light is green, the secondary is red, and vice versa.

The code is split into two responsibilities deliberately. `lightsTimer()` handles only state transitions: it checks whether enough time has passed to move to the next phase, and if so, updates `color`. It never touches the physical pins. `lightsChecker()` handles only physical output: it reads the current `color` and sets every LED accordingly. This separation means the LEDs are always an accurate reflection of the program's internal state, with no timing dependency.

Instead of `delay()`, all timing is done with `millis()`, which returns the number of milliseconds since the Arduino powered on. A `previousMillis` variable stores the timestamp of the last state transition, and the difference between that and the current time is compared against each phase's interval. This keeps the microcontroller awake and responsive at all times.

---

## The pedestrian button

The button works bidirectionally. Pressing it while the main light is green forces a transition to yellow, and the `traffic` variable is set to `2`, telling the yellow phase to resolve toward red. Pressing it while the main light is red does the opposite — `traffic` is set to `1`, and yellow resolves toward green. This way a single button serves both sides of the intersection.

Edge detection is used so the button only registers once per physical press, regardless of how long it is held. This is done by comparing the current button value against `previousVal` from the last loop iteration.

---

## Hardware

- Arduino Uno
- 2× Red LEDs
- 2× Yellow LEDs  
- 2× Green LEDs
- 6× 220Ω resistors (one per LED)
- 1× Pushbutton
- 1× 10kΩ resistor (pull-down for the button)
- Breadboard and jumper wires

---

## Wiring

![Circuit diagram](<img width="1098" height="768" alt="Captura de tela 2026-04-05 034339" src="https://github.com/user-attachments/assets/86ec6afb-9a6d-4594-a7ab-0ebe6adde8ee" />)


The main traffic light uses pins 3 (red), 4 (yellow), and 5 (green). The secondary traffic light uses pins 6 (red), 7 (yellow), and 8 (green). The pedestrian button is connected to pin 2, with a 10kΩ pull-down resistor to GND and the opposite leg connected to 5V. This ensures the pin reads LOW by default and only goes HIGH when the button is physically pressed.

---

## Phase intervals

The duration of each phase is defined as constants at the top of the sketch, making them easy to adjust without touching any logic.

| Phase  | Duration |
|--------|----------|
| Green  | 5000 ms  |
| Yellow | 1500 ms  |
| Red    | 5000 ms  |

---

## Key concepts used

This project was built as a practical exercise in several embedded systems fundamentals. Non-blocking timing with `millis()` was the central constraint — understanding why `delay()` makes a microcontroller unresponsive is what motivates the entire architecture. The state machine pattern (separating *when* to transition from *what to output*) is something that scales directly to more complex systems like animatronics or robotics. Edge detection on the button input is necessary any time you want a held button to register as a single event rather than thousands of repeated triggers per second.

---

## What I'd add next

With servo motors, the same state machine pattern could drive physical barrier arms at the crosswalk — one arm lowers when the pedestrian phase is active and rises when traffic resumes. The `lightsChecker()` function would simply gain additional cases for the servo positions alongside the LED logic.

## Hardware notes

The standard recommendation for LED current-limiting resistors on a 5V 
Arduino is 220Ω, which drives LEDs at roughly 14mA and full brightness. 
This build uses 4.7kΩ resistors instead due to availability, which reduces 
current to around 0.6mA. The LEDs light up visibly but at reduced brightness 
— acceptable for a prototype where verifying state transitions matters more 
than luminosity.

For the button pull-down, three 4.7kΩ resistors were wired in series to 
produce approximately 14.1kΩ. The pull-down value isn't critical — anything 
between 10kΩ and 100kΩ reliably holds the input pin LOW when the button is 
open, while still allowing the 5V signal to pull it HIGH when pressed. 
14.1kΩ falls well within that range and behaves identically to a standard 
10kΩ resistor in this application.

## Known Issues

**Yellow phase on button press doesn't match real traffic light behavior**

In a real intersection, a traffic light transitions from red directly to green
without passing through yellow — yellow only appears between green and red as a
warning that the light is about to stop. In this system, pressing the pedestrian
button always forces both lights into yellow simultaneously before resolving,
which means a light that was already red will incorrectly show yellow before
turning green.

The reason this happens is that the system uses a single `color` variable to
represent both lights at once. Yellow is a shared intermediate state, so there
is currently no way to transition one light independently of the other.

A proper fix would require each traffic light to track its own state separately,
which introduces a synchronization problem: if the main light jumps from red to
green directly while the secondary is mid-transition, the two lights can fall
out of their expected inverse relationship. Solving this would likely require
a more complex state machine where the button triggers a controlled
resynchronization sequence rather than an immediate phase change.
