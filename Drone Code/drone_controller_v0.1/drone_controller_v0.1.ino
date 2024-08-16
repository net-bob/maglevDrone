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
int joyMax = 1920;                  // Min and max values are ONLY for the left joystick. Right joystick versions are (very roughly) reversed.
int joyMin = 950;

unsigned long leftJoyY;
unsigned long rightJoyY;
int yDirection;

int getDirection(unsigned long pulseWidth) {
  if (joyMid - directionDeadzone < pulseWidth && pulseWidth < joyMid + directionDeadzone) { // if joystick is within deadzone
    return 0;
  } 
  else if (pulseWidth > joyMid + directionDeadzone) {
    /* if joystick is "above" deadzone 
     * return "down" direction since right joystick is reversed
    */ 
    return -1;
  } 
  else if (pulseWidth < joyMid - directionDeadzone) {
    /* if joystick is "below" deadzone writeMotor
     * return "up" direction since right joystick is reversed
    */
    return 1;
  }
}

int average(int a, int b){
  return round((double(a) + double(b)) / 2.0);
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

void writeMotor(Servo esc, int yDirection){
  int escWrite = motorFunction(esc, yDirection).toInt();
  esc.writeMicroseconds(escWrite);
}

String motorFunction(Servo esc, int yDirection){
  if (yDirection == 1 && leftJoyY > joyMin + speedDeadzone) { // if direction is forward & stick is above deadzone
    return String(map(leftJoyY, joyMin, joyMax, escMid, escMax)); // translate joystick Y position to ESC input range and write to corresponding ESC
  } 
  else if (yDirection == -1 && leftJoyY > joyMin + speedDeadzone) {
    return String(map(leftJoyY, joyMin, joyMax, escMid, escMin));
  } 
  else {
    return String(escMid);
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
  buttonPressed = (pulseIn(buttonPin, HIGH) > 1100) ? true : false;

  if (buttonPressed && !buttonHeld) {
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

  if (powered) {
    if (leftJoyY < joyMin) {
      joyMin = leftJoyY;
    }
    if (leftJoyY > joyMax) {
      joyMax = leftJoyY;
    }

    yDirection = getDirection(rightJoyY);
    writeMotor(lowerLeftESC, yDirection);
    writeMotor(lowerRightESC, yDirection);
    writeMotor(upperLeftESC, -yDirection);
    writeMotor(upperRightESC, -yDirection);

    Serial.println("Left joystick Y: " + String(leftJoyY) + ", Right joystick direction: " + String(getDirection(rightJoyY)) + ", Speed: " + motorFunction(upperRightESC, yDirection));
  }
}
