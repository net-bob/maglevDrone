#include <Servo.h>

Servo ky;
int duration = 45;
int delayTime = 15;

void setup() {
  Serial.begin(115200);
  ky.attach(9);
  ky.write(100);
}

void loop() {

  ky.write(180);

  for (int i = 0; i <= 180; i++){
    Serial.println(String(i) + " " + String(ky.read()));
    ky.write(i);

    delay(15);
  }
  for (int i = 180; i >= 0; i--){
    Serial.println(String(i) + " " + String(ky.read()));
    ky.write(i);

    delay(15);
  }
  
  ky.write(15);
  delay(1000);
  ky.write(0);
  delay(1000);



}
