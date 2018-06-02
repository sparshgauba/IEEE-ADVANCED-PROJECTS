//Arduino Side of things
#include <printf.h>
#include <SoftwareSerial.h>
#include "RF24.h"

#define CEPIN 9
#define CSPIN 10

SoftwareSerial mySerial (3,4);// (Rx, Tx)
char outgoing = 0;
uint8_t* chrptr = (uint8_t*)&outgoing;
int red; // a variable to read incoming serial data into
int lastRed = LOW;

int green;
int lastGreen = LOW;

int yellow;
int lastYellow = LOW;

long lastDebounceTime;
long debounceDelay = 5;

struct Data
{
    uint8_t num;
};

Data incoming = {0};
Data transmit = {69};
uint8_t* txptr = (uint8_t*)&transmit;
uint8_t* rxptr = (uint8_t*)&incoming;

RF24 radio(CEPIN, CSPIN);

void setup()
{
    // initialize serial communication:
  
    printf_begin();
    Serial.begin(9600);
    SPI.begin();
    mySerial.begin(9600);
    radio.begin();
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
  
    radio.setChannel (26);
    radio.setPALevel (RF24_PA_MIN);
    radio.openWritingPipe (0xFFFF0000F0);
    radio.openReadingPipe (1, 0x0000FFFF0F);
    radio.setCRCLength(RF24_CRC_8);
  
    delay(125);
  
    radio.printDetails();
    printf("\n\n%d\n", radio.getPayloadSize());
}

char inputButton()
{
  int five;
  int six;
  int seven;

  char output = 0;

  while(1)
  {
    five = digitalRead(5);
    six = digitalRead(6);
    seven = digitalRead(7);
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
      lastRed = red;
      lastGreen = green;
      lastYellow = yellow;
      return 'R';
    }
    if(green == LOW && lastGreen == HIGH)
    {
      lastRed = red;
      lastGreen = green;
      lastYellow = yellow;
      return 'G';
    }
    if(yellow == LOW && lastYellow == HIGH)
    {
      lastRed = red;
      lastGreen = green;
      lastYellow = yellow;
      return 'Y';
    }
    
    lastRed = red;
    lastGreen = green;
    lastYellow = yellow;
  }
}

void loop()
{
    Serial.println("TEST MESSAGE LENSE FLAIR");
    //delay(125);
    outgoing = inputButton();
    
    if(radio.write(chrptr, 8))
    {
        Serial.println("successfully sent.");
    }
    else
        Serial.println("Not successful");
    //Serial.println("SENT\n");
    printf("%c\n", outgoing);
    
    /*
    radio.startListening();
    Serial.println("listening..");
    while (!radio.available())
    {
        //Serial.println("Listening..");
    }
    Serial.println("Found Something..");
    radio.read(rxptr, 8);
    radio.stopListening();
    if(incoming.num == 42)
    {
      printf("Things went well.\n");
      printf("value = %d", incoming.num);
    }
    
    else
    {
      printf("Failure.\n");
    }
  
    // put your main code here, to run repeatedly:
    */
}






























