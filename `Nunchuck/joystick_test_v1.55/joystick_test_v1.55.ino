#include <WiiChuck.h>

int deadzone = 10;  // This is how far the joystick must be away from the starting position in order to register movement.
Accessory nunchuck;
const int joyMiddle = 128;
const int joyMax = 255;
const int joyMin = 0;

int getDirection(byte val) {
  if (joyMiddle - deadzone < val && val < joyMiddle + deadzone) { // if nunchuck is within deadzone
    return 0;
  } else if (val < joyMiddle - deadzone) { // if nunchuck is "below" deadzone (negative x/y)
    return -1;
  } else if (joyMiddle + deadzone < val) { // if nunchuck is "above" deadzone (positive x/y)
    return 1;
  }
}

// both x & y pins must be PWM capable *NOTE: Mega board does not have the problem anymore
const int xPin = 5;
const int yPin = 6;

void setup() {
  Serial.begin(115200);
  nunchuck.begin();
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  nunchuck.readData();

  int xSign = getDirection(nunchuck.getJoyX());  // int value between -1 and 1. For now, there is no steering control (full turning or nothing)
  int ySign = getDirection(nunchuck.getJoyY());  // int value between -1 and 1. For now, there is no speed control (full throttle or nothing)

  Serial.println("Joystick X: " + String(nunchuck.getJoyX()) + ", Joystick Y: " + String(nunchuck.getJoyY()));
  //Serial.println(map(joystickY, 129 + deadzone, 226, 0, 255));

  // X Direction on Joystick
  // Uses one pin. Pin voltage increases (using PWM) when joystick is moved right or left.
  if (getDirection(nunchuck.getJoyX()) == 1) {                               // get direction of joystick (in this case, up)
    analogWrite(xPin, map(nunchuck.getJoyX(), joyMiddle + deadzone, joyMax, 0, 255)); // translate joystick coordinate to analogWrite range and adjust voltage accordingly
  } else if (getDirection(nunchuck.getJoyX()) == -1) {
    analogWrite(xPin, map(nunchuck.getJoyX(), joyMiddle - deadzone, joyMin, 0, 255));
  } else if (getDirection(nunchuck.getJoyX()) == 0) {
    analogWrite(xPin, 0);
  }

  // Y Direction on Joystick
  // Uses one pin. Pin voltage increases (using PWM) when joystick is moved up or down.
  if (getDirection(nunchuck.getJoyY()) == 1) {                                
    analogWrite(yPin, map(nunchuck.getJoyY(), joyMiddle + deadzone, joyMax, 0, 255));
  } else if (getDirection(nunchuck.getJoyY()) == -1) {
    analogWrite(yPin, map(nunchuck.getJoyY(), joyMiddle - deadzone, joyMin, 0, 255));
  } else if (getDirection(nunchuck.getJoyY()) == 0) {
    analogWrite(yPin, 0); 
  }

  // Back Buttons on Remote
  // Uses two pins. Pin 7 activates whenever the Z Button is pressed. Pin 8 activates whenever the C Button is pressed.
  digitalWrite(7, nunchuck.getButtonZ());
  digitalWrite(8, nunchuck.getButtonC());
}