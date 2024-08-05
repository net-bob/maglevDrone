void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT); // LED out
  pinMode(2, INPUT);   // button in
  pinMode(4, INPUT);   // LED in
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(2) == HIGH) {       // if button pressed...
    if (digitalRead(4) == HIGH) {    // & LED on
      digitalWrite(12, LOW);         // turn LED off
    }
    else {                           // & LED off
      digitalWrite(12, HIGH);        // turn LED on
    }

    while(digitalRead(2) == HIGH) {  // while button held down
      delay(100);                    // pause program to prevent feedback loop/LED blinking
    }
  }
}
