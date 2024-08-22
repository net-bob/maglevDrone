#include <Servo.h>

bool powered = false;
bool buttonHeld = false;
bool buttonPressed = false;

Servo lowerLeftESC;
Servo lowerRightESC;
Servo upperLeftESC; 
Servo upperRightESC;
const int escMin = 930;
const int escMid = 1465;
const int escMax = 2005;

const int leftJoystickYPin = 8;
const int rightJoystickYPin = 9;

const int directionDeadzone = 100;  // This is how far the right joystick must be from the middle in order to register movement.
const int speedDeadzone = 43;       // This is how far the left joystick must be from the bottom in order to register movement.
int joyMid = 1470;
int joyMax = 1920;    // Min and max values are ONLY for the left joystick. Right joystick versions are (very roughly) reversed.
int joyMin = 950;
int joyMinLift = 1750;  // Minimum left joystick value needed to generate a reasonable amount of lift. (PLACEHOLDER- actual value TBD)

unsigned long leftJoyY;
unsigned long rightJoyY;
int yDirection;

int getDirection(unsigned long pulseWidth) { // uses right joystick
  if (joyMid - directionDeadzone < pulseWidth && pulseWidth < joyMid + directionDeadzone) { // if joystick is within deadzone
    return 0;
  } 
  else if (pulseWidth > joyMid + directionDeadzone) {  // if joystick is "above" deadzone
    return -1;                                         // return "down" direction since right joystick is reversed
  } 
  else if (pulseWidth < joyMid - directionDeadzone) {  // if joystick is "below" deadzone
    return 1;                                          // return "up" direction since right joystick is reversed
  }
}

int calculateRotationsPerMinute(int pin){
  float deltaTime = pulseIn(A0, LOW);
  deltaTime /= 60000000.0;
  return (1 / deltaTime);
}

int calculateRotationsPerSecond(int pin){
  float deltaTime = pulseIn(A0, LOW);
  deltaTime /= 1000000.0;
  return (1 / deltaTime);
}

// all pins must be PWM capable
const int buttonPin = 2;
const int lowerLeftPin = 4;
const int lowerRightPin = 5;
const int upperLeftPin = 6;
const int upperRightPin = 7;

void writeMotor(Servo esc){ // When the power button is off, this function sends a message to the motors, turning them off.
  esc.writeMicroseconds(joyMid);
}

void writeMotor(Servo lowerLeftESC, Servo lowerRightESC, Servo upperLeftESC, Servo upperRightESC, int yDirection) {   // writes translated speeds (from motorFunction) to all ESCs
  lowerLeftESC.writeMicroseconds(backMotorFunction(yDirection).toInt());
  lowerRightESC.writeMicroseconds(backMotorFunction(yDirection).toInt());
  upperLeftESC.writeMicroseconds(frontMotorFunction(yDirection).toInt());
  upperRightESC.writeMicroseconds(frontMotorFunction(yDirection).toInt());
}

String backMotorFunction(int yDirection) { // uses left joystick
  if (yDirection == 1 && leftJoyY > joyMin + speedDeadzone) {                  // if direction is forward & stick is above deadzone
    int liftedRangeInput = map(leftJoyY, joyMin, joyMax, joyMinLift, joyMax);  // translate joystick Y position to range above minimum throttle for lift
    return String(map(liftedRangeInput, joyMinLift, joyMax, escMid, escMax));  // translate that modified joystick position to ESC forward input range & output as String
  } 
  else if (yDirection == -1 && leftJoyY > joyMin + speedDeadzone) {
    return String(map(leftJoyY, joyMin, joyMax, escMid, escMin));  // translate speed from forward joyMinLift to reversed max based on joystick movement, with workaround for extremely slow speeds to avoid motor dip/stop
  } 
  else {                                                               // if no direction (stationary)
    return String(map(joyMinLift, joyMin, joyMax, escMid, escMax));    // translate minimum throttle level required to generate lift to ESC forward input range
  }
}

String frontMotorFunction(int yDirection) { // uses left joystick
  if (yDirection == 1 && leftJoyY > joyMin + speedDeadzone) {     // if direction is forward & stick is above deadzone
    return String(map(leftJoyY, joyMin, joyMax, escMid, escMax)); // translate speed from reversed joyMinLift to forward max based on joystick movement, with workaround for extremely slow speeds to avoid motor dip/stop
  } 
  else if (yDirection == -1 && leftJoyY > joyMin + speedDeadzone) {
    return String(map(leftJoyY, joyMin, joyMax, escMid, escMin)); // translate speed from reversed joyMinLift to reversed max based on joystick movement
  } 
  else {                                                               // if no direction (stationary)
    return String(map(joyMinLift, joyMin, joyMax, escMid, escMin));    // translate minimum throttle level required to generate lift to ESC backward input range
  }
}

void setup() {
  Serial.begin(9600);
  lowerLeftESC.attach(lowerLeftPin);
  lowerRightESC.attach(lowerRightPin);
  upperLeftESC.attach(upperLeftPin);
  upperRightESC.attach(upperRightPin);
}

void loop() {
  leftJoyY = pulseIn(leftJoystickYPin, HIGH);
  rightJoyY = pulseIn(rightJoystickYPin, HIGH);
  buttonPressed = (pulseIn(buttonPin, HIGH) > 1100) ? true : false; // one-line if statement: if button is pressed, return true, else return false

  if (buttonPressed && !buttonHeld) { // if button is pressed, toggle "powered" variable (on/off button logic)
    buttonHeld = true;
    if (!powered) {
      powered = true;
    }
    else {
      powered = false;

      writeMotor(lowerLeftESC);
      writeMotor(lowerRightESC);
      writeMotor(upperLeftESC);
      writeMotor(upperRightESC);
    }
  }
  else if (!buttonPressed) {
    buttonHeld = false;
  }

  if (powered) { // if drone is on run this (main loop code)
    if (leftJoyY < joyMin) {
      joyMin = leftJoyY;
    }                           // adaptive joystick parameter calculation. makes speed control slightly more precise
    if (leftJoyY > joyMax) {
      joyMax = leftJoyY;
    }

    yDirection = getDirection(rightJoyY);   // get direction from right joystick (forward, backward, middle)
    writeMotor(lowerLeftESC, yDirection);   // write speed to ESCs according to left joystick position (unless direction is middle)
    writeMotor(lowerRightESC, yDirection);
    writeMotor(upperLeftESC, yDirection);
    writeMotor(upperRightESC, yDirection);

    Serial.println("Left joystick Y: " + String(leftJoyY) + ", Right joystick direction: " + String(getDirection(rightJoyY)) + ", Speed: " + motorFunction(upperRightESC, yDirection));
  }
}
