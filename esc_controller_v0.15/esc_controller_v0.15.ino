/*
Coded by Marjan Olesch
Sketch from Insctructables.com
*/

#include <Servo.h>

int value = 0; // initialize the variables you need
Servo firstESC, secondESC; // you can control 2 or more servos simultaneously

void setup() {

 firstESC.attach(9);   // attached to pin 9
 Serial.begin(9600);   // start serial at 9600 baud
 firstESC.write(100);

}

void loop() {

// First connect your ESC WITHOUT arming. Then open the serial and follow the instructions.
 firstESC.write(value);

 if(Serial.available()) 
   value = Serial.parseInt();   // parse an integer from serial
}