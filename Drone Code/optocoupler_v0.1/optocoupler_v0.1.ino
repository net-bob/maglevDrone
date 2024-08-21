void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  // NOTE: If the program isn't working, try reuploading the program to the Arduino or hitting the reset button.
  // millis() resets after 50 days and I'm not sure if that breaks the code or not.
  float deltaTime = pulseIn(A0, LOW);
  deltaTime /= 60000000.0; // Per Minute
  // deltaTime /= 1000000; // Per Second
  Serial.println(1 / deltaTime);
}
