#include <SoftwareSerial.h>
SoftwareSerial mySerial (3,4);// (Rx, Tx)
int red; // a variable to read incoming serial data into
int lastRed = LOW;

int green;
int lastGreen = LOW;

int yellow;
int lastYellow = LOW;

long lastDebounceTime;
long debounceDelay = 5;


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}

void loop() {
  int five = digitalRead(5);
  int six = digitalRead(6);
  int seven = digitalRead(7);
  
  delay(10);
  
  if (five == digitalRead(5))
  {
    red = five;
  }
  if (six == digitalRead(6))
  {
    green = six;
  }
  if (seven == digitalRead(7))
  {
    yellow = seven;
  }
  
  if(red == LOW && lastRed == HIGH)
  {
   mySerial.write('R');
   Serial.println('R');
  }
  if(green == LOW && lastGreen == HIGH)
  {
   mySerial.write('G');
   Serial.println('G');
  }
  if(yellow == LOW && lastYellow == HIGH)
  {
   mySerial.write('Y');
   Serial.println('Y');
  }
  
  lastRed = red;
  lastGreen = green;
  lastYellow = yellow;
}
