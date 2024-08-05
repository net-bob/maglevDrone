#include <WiiChuck.h>

Accessory nunchuck;

void setup() {
	Serial.begin(115200);
	nunchuck.begin();
	if (nunchuck.type == Unknown) {
		nunchuck.type = NUNCHUCK;
	}
}

void loop() {
  nunchuck.readData();    // Read inputs and update maps

  Serial.print("X: "); Serial.print(nunchuck.getAccelX());
  Serial.print(" \tY: "); Serial.print(nunchuck.getAccelY()); 
  Serial.print(" \tZ: "); Serial.println(nunchuck.getAccelZ()); 

  Serial.print("Joy: ("); 
  Serial.print(nunchuck.getJoyX());
  Serial.print(", "); 
  Serial.print(nunchuck.getJoyY());
  Serial.println(")");

  Serial.print("Button: "); 
  if (nunchuck.getButtonZ()) Serial.print(" Z "); 
  if (nunchuck.getButtonC()) Serial.print(" C "); 

  Serial.println();
  delay(100);
}