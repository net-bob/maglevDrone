int counter = 0;
const int size = 7;
bool output[size];

int largestExpOf2(int n){
  int exp = 0;
  while (pow(2, exp) <= n){
    exp++;
  }
  return exp - 1;
}

void base10ToBase2(int n){
  for (int i = 0; i < size; i++){
    output[i] = 0;
  }
  int number = n;
  int exp = largestExpOf2(number);
  while (number != 0){
    output[exp] = 1;
    number -= pow(2, exp);
    exp = largestExpOf2(number);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  for (int i = 12; i > 12 - size; i--){
    pinMode(i, OUTPUT);
  }
}

void loop() {
  
  if (digitalRead(2)){
    counter++;
    if (counter == pow(2, size)){
      counter = 0;
    }
  }
  
  while (digitalRead(2));

  base10ToBase2(counter);
  for (int pin = 12; pin > 12 - size; pin--){
    digitalWrite(pin, output[pin - (12 - size + 1)]);
  }
  
  Serial.print("Counter " + String(counter) + " ");
  for (int i = 0; i < size; i++){
    Serial.print(output[i]);
  }
  Serial.println();
}