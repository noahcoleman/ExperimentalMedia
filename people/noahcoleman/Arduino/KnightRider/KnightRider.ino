//Knight Rider Sketch

int del=100; // sets a default delay time
int ledPins[] = {2,3,4,5,6,7,8};//creates an array to access led pins 2-8
void setup() {
  // initialize the digital pins as outputs:
  for (int i = 2; i<=8 ; i++) {
    pinMode(i, OUTPUT);
  } // end of for loop
} // end of setup

void loop() {
  for (int i = 2; i<=8; i++) { // blink from LEDs 2 to 8
    digitalWrite(i, HIGH);
    delay(del);
    digitalWrite(i, LOW);
  }
  for (int i = 7; i>=3; i--) { // blink from LEDs 8 to 3
    digitalWrite(i, HIGH);
    delay(del);
    digitalWrite(i, LOW);
  }
}

