#include <Servo.h>

byte servoPin = 9;
byte potentiometerPin = A2;
Servo servo;


void setup() {
  Serial.begin(9600);

  pinMode(2, INPUT);
  pinMode(12, OUTPUT);

  servo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  int potVal = analogRead(potentiometerPin)
  analogWrite(12, map(potVal, 0, 1023, 0, 255));

  int pwmVal = map(potVal, 0, 1023, 1100, 1900);
  servo.writeMicroseconds(pwmVal);
}