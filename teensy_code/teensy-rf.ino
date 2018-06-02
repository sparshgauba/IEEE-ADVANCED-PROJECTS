//Teensy Side of things
#include <printf.h>
#include "RF24.h"

#define CEPIN 9
#define CSPIN 10

char character = 0;
uint8_t* chrptr = (uint8_t*)&character;

struct Data
{
    uint8_t num;
};

Data incoming = {0};
Data transmit = {42};
uint8_t* txptr = (uint8_t*)&transmit;
uint8_t* rxptr = (uint8_t*)&incoming;

RF24 radio(CEPIN, CSPIN);

void setup()
{
    Serial1.begin(9600);
    Serial.begin(9600);
    SPI.begin();
    printf_begin();
    radio.begin();
    pinMode(14, OUTPUT); // RED (BROKEN NOW)
    pinMode(15, OUTPUT); // GREEN
    pinMode(16, OUTPUT); // YELLOW
    
    radio.setChannel (26);
    radio.setPALevel (RF24_PA_MIN);
    radio.openWritingPipe (0x0000FFFF0F);
    radio.openReadingPipe (1, 0xFFFF0000F0);
    radio.setCRCLength(RF24_CRC_8);

    delay(1250);
    
    radio.printDetails();
    printf("\n\n%d\n", radio.getPayloadSize());
    radio.startListening();
}

void lightup (char led)
{
  switch (led)
  {
    case 'R':
    {
      digitalWrite(14, HIGH);
          delay(500);
          digitalWrite(14, LOW);
          delay(250);
          break;
    }
    case 'r':
      digitalWrite(14, !digitalRead(14));
          break;
    case 'G':
    {
      digitalWrite(15, HIGH);
          delay(500);
          digitalWrite(15, LOW);
          delay(250);
          break;
    }
    case 'g':
      digitalWrite(15, !digitalRead(15));
          break;
    case 'Y':
    {
      digitalWrite(16, HIGH);
          delay(500);
          digitalWrite(16, LOW);
          delay(250);
          break;
    }
    case 'y':
      digitalWrite(16, !digitalRead(16));
          break;
  }
}

void loop()
{
    character = '\0';
    //delay(125);
    Serial.println("TEST MESSAGE LENSE FLAIR");
    
    Serial.println("listening..");
    while (!radio.available())
    {
      //Serial1.println("listening..");
    }
    Serial.println("found something..");
    radio.read(chrptr, 8);
    printf("teensy value = %c\n", character);
    //radio.stopListening();
    if(character != '\0')
    {
      //Serial.println ("Things went well.");
      printf("teensy value = %c", character);
      lightup(character);
      //lightup('R');
    }
    /*
    else
    {
      Serial.println ("Failure.");
      lightup('Y');
      lightup('Y');
    }
    lightup('R');
    delay(1000);
    printf("\nSending..\n");
    radio.write(txptr, 8);
    printf("\nSENT\n");
    */
}






























