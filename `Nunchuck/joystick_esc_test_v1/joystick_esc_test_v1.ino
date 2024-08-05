#include <Servo.h>

Servo lowerLeftESC;
Servo lowerRightESC;
Servo upperLeftESC; 
Servo upperRightESC;
const int escMin = 920;
const int escMid = 1460;
const int escMax = 2000;

int deadzone = 5;  // This is how far the joystick must be away from the starting position in order to register movement.
const int joyMiddle = 128;
const int joyMax = 255;
const int joyMin = 0;

int yDirection;

int getDirection(byte val) {
  if (joyMiddle - deadzone < val && val < joyMiddle + deadzone) { // if nunchuck is within deadzone
    return 0;
  } else if (val < joyMiddle - deadzone) { // if nunchuck is "below" deadzone (negative x/y)
    return -1;
  } else if (joyMiddle + deadzone < val) { // if nunchuck is "above" deadzone (positive x/y)
    return 1;
  }
}

// all pins must be PWM capable
const int lowerLeftPin = 4;
const int lowerRightPin = 5;
const int upperLeftPin = 6;
const int upperRightPin = 7;

void motorFunction(Servo esc, int yDirection){
  if (yDirection == 1) {
    esc.writeMicroseconds(map(nunchuck.getJoyY(), joyMiddle + deadzone, joyMax, escMid, escMax));
  } 
  else if (yDirection == -1) {
    esc.writeMicroseconds(map(nunchuck.getJoyY(), joyMiddle - deadzone, joyMin, escMid, escMin));
  } 
  else if (yDirection == 0) {
    esc.writeMicroseconds(escMid);
  }
}

void setup() {
  Serial.begin(115200);
  lowerLeftESC.attach(lowerLeftPin);
  lowerRightESC.attach(lowerRightPin);
  upperLeftESC.attach(upperLeftPin);
  upperRightESC.attach(upperRightPin);
  nunchuck.begin();
}

void loop() {
  nunchuck.readData();

  Serial.println("Joystick X: " + String(nunchuck.getJoyX()) + ", Joystick Y: " + String(nunchuck.getJoyY()));

  yDirection = getDirection(nunchuck.getJoyY());
  motorFunction(lowerLeftESC, yDirection);
  motorFunction(lowerRightESC, yDirection);
  motorFunction(upperLeftESC, yDirection);
  motorFunction(upperRightESC, yDirection);

  // // Back Buttons on Remote
  // // Uses two pins. Pin 7 activates whenever the Z Button is pressed. Pin 8 activates whenever the C Button is pressed.
  // digitalWrite(7, nunchuck.getButtonZ());
  // digitalWrite(8, nunchuck.getButtonC());
}
