// Variables and Outputs/Inputs

// Switch Inputs and variable for bi-directional switch
const int switchIn = 2; // switch
int traffic = 2; // Variable for which Traffic Light will turn red when pressed
int previousVal = LOW; // Tells if the switch was last pressed or not

// Main traffic Lights Outputs
const int redOut = 3;
const int yellowOut = 4;
const int greenOut = 5;

// Secondary traffic Lights Outputs
const int redOut_2 = 6;
const int yellowOut_2 = 7;
const int greenOut_2 = 8;

// Color Intervals and variables
char color = 'g'; // Declared as g since the main traffic starts as green
const long gInterval = 5000; // green Interval
const long yInterval = 1500; // yellow Interval
const long rInterval = 5000; // red Interval

// Calculate previous time and 
unsigned long previousMillis = 0;

// Declaring Functions
void lights();
void lightsChecker();

void setup() {
  // Led that is on the Arduino
  pinMode(LED_BUILTIN, OUTPUT);

  // Switch
  pinMode(switchIn, INPUT);

  // Main traffic Light
  pinMode(redOut, OUTPUT);
  pinMode(yellowOut, OUTPUT);
  pinMode(greenOut, OUTPUT);

  // Secondary Traffic Light
  pinMode(redOut_2, OUTPUT);
  pinMode(yellowOut_2, OUTPUT);
  pinMode(greenOut_2, OUTPUT);
} // setup()

void loop() {
  int val = digitalRead(switchIn); // reads the switch Input

  // Checks the current state of both traffic lights when the button is pressed, working as a pedestrian button 
  if (val == HIGH && previousVal == LOW) {
    if (color == 'r') {
      // Check if the main one is red, if pressed then it goes green
      color = 'y';
      traffic = 1;
      previousMillis = millis(); // save the time when button was pressed
    } else if (color == 'g') {
      // Check if the main one is green, if pressed then it goes red
      color = 'y';
      traffic = 2;
      previousMillis = millis();
    }
    digitalWrite(LED_BUILTIN, HIGH); // turns on the LED builtin 
  } 

  if (val == LOW) {
    digitalWrite(LED_BUILTIN, LOW);
  } // Turns off the Led when button is not pressed
 
  lightsTimer(); // Makes the cycle of the LEDS
  lightsChecker(); // Check if the colors matches the current state
  previousVal = val; // save the last value of the button
}

void lightsTimer() {
  // Set the current time since the program started
  unsigned long time = millis();
  unsigned long variation = time - previousMillis;

  /* This part selects which color the current state of the main traffic light color is, then it checks the variation of time between the current time and the last time the cycle before ended.
  When the variation is bigger or equals to the inteval, then it can go to the next cycle.
  Example: if 5000ms have passed after the start of the program, the difference between 5000ms and 0ms is 5000ms, so it matches the green Interval and now it saves the 5000ms to the previousValue.*/

  switch (color) {
    case 'g': // When the Main Traffic is Green
      if (variation >= gInterval) {
        previousMillis = time; // save the time when the condition is matched
        color = 'y'; // then it goes to the next color
      }
      break;

    case 'y': // When the Main Traffic is Yellow
      if (variation >= yInterval) {
        previousMillis = time;
        color = (traffic == 1) ? 'g' : 'r'; // Checks if the main traffic is currently red or green. If it is red, it goes to green, and if it is green, it goes to red.
        traffic = 2; // After the color change, it goes back to default where the main traffic is green
      }
      break;

    case 'r': // When the Main Traffic is Red
      if (variation >= rInterval) { 
        previousMillis = time;
        color = 'g';
      }
      break;
  } // switch
} // lightsTimer

void lightsChecker () {
  // Checks the current Color for each case
  switch (color) {
    case 'g':
      // Traffic Light - Main (Is Green)
      digitalWrite(greenOut, HIGH);
      digitalWrite(yellowOut, LOW);
      digitalWrite(redOut, LOW);

      // Traffic Light - Secondaru (Is Red)
      digitalWrite(greenOut_2, LOW);
      digitalWrite(yellowOut_2, LOW);
      digitalWrite(redOut_2, HIGH); 
      break;

    case 'y':
      // Traffic Light - Main (Is Yellow)
      digitalWrite(greenOut, LOW);
      digitalWrite(yellowOut, HIGH);
      digitalWrite(redOut, LOW);

      // Traffic Light - Secondary (Is Yellow)
      digitalWrite(greenOut_2, LOW);
      digitalWrite(yellowOut_2, HIGH);
      digitalWrite(redOut_2, LOW); 
      break;

    case 'r':
      // Traffic Light - Main (Is Red)
      digitalWrite(greenOut, LOW);
      digitalWrite(yellowOut, LOW);
      digitalWrite(redOut, HIGH);

      // Traffic Light - Secondary (Is Green)
      digitalWrite(greenOut_2, HIGH);
      digitalWrite(yellowOut_2, LOW);
      digitalWrite(redOut_2, LOW); 
      break;
  } // switch
}// lightsChecker