#include <WiiChuck.h>
#include <Servo.h>

Accessory nunchuck;
Servo ky;
int duration = 45;
int delayTime = 15;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nunchuck.begin();
  ky.attach(9);
  ky.write(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("Z Button: " + nunchuck.getButtonZ());

  ky.write(180);

  // if (nunchuck.getButtonZ() == 1){
  // for (int i = 0; i <= 180; i++){
  //   Serial.println(String(i) + " " + String(ky.read()));
  //   // ky.write(90);
  //   ky.write(i);

  //   delay(15);
  // }
  // for (int i = 180; i >= 0; i--){
  //   Serial.println(String(i) + " " + String(ky.read()));
  //   // ky.write(90);
  //   ky.write(i);

  //   delay(15);
  // }
  // }
  
  // ky.write(15);
  // delay(1000);
  // ky.write(0);
  // delay(1000);



}
