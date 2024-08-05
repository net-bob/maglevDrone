#include <Servo.h>

Servo upperRightESC;

const int outputMax = 255;
const int outputMin = 0;
int minThrottleVal = 9999;
int maxThrottleVal = 0;
int minElevatorVal = 9999;
int maxElevatorVal = 0;
const int escMin = 920;
const int escMid = 1460;
const int escMax = 2000;
const int deadzone = 30;123213

int getDirection(unsigned long val){
  int middleVal = average(minElevatorVal, maxElevatorVal);

  if (val > middleVal + deadzone){
    return 1;
  }
  else if (val < middleVal - deadzone){
    return -1;
  }
  else {
    return 0;
  }
}

int average(int a, int b){
  return round((double(a) + double(b)) / 2.0);
}

void motorFunction(Servo esc, int yDirection, unsigned long write){
  unsigned long valueToBeWritten;
  if (yDirection == 1){
    valueToBeWritten = map(write, minThrottleVal, maxThrottleVal, escMid, escMax);
    Serial.println(valueToBeWritten);
    if (abs(valueToBeWritten - escMid) >= 20){
      esc.writeMicroseconds(map(write, minThrottleVal, maxThrottleVal, escMid, escMax));
    }
  }
  else if (yDirection == -1){
    valueToBeWritten = map(write, minThrottleVal, maxThrottleVal, escMid, escMin);
    Serial.println(valueToBeWritten);
    if (abs(escMid - valueToBeWritten) >= 20){
      esc.writeMicroseconds(map(write, minThrottleVal, maxThrottleVal, escMid, escMin));
    }
  }
  else {
    Serial.println(escMid);
    esc.writeMicroseconds(escMid);
  }
}

void setup() {
  // put your setup code here, to run once:
  upperRightESC.attach(7);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long storedPulse8 = pulseIn(8, HIGH);
  unsigned long storedPulse9 = pulseIn(9, HIGH);

  if (storedPulse8 < minThrottleVal){
    minThrottleVal = storedPulse8;
  }
  if (storedPulse8 > maxThrottleVal){
    maxThrottleVal = storedPulse8;
  }
  if (storedPulse9 < minElevatorVal){
    minElevatorVal = storedPulse9;
  }
  if (storedPulse9 > maxElevatorVal){
    maxElevatorVal = storedPulse9;
  }

  motorFunction(upperRightESC, getDirection(storedPulse9), storedPulse8);
  // Serial.println("Middle Val: " + String(average(minElevatorVal, maxElevatorVal)) + ", Direction: " + String(getDirection(storedPulse6)) + ", Raw: " + String(storedPulse6));
  if (Serial.available()){
    while (true){
      
    }
  }

  Serial.println("Left Joystick: " + String(storedPulse8) + ", Right Joystick: " + String(storedPulse9) + ", Right Joystick Direction: " + getDirection(storedPulse9));
}
