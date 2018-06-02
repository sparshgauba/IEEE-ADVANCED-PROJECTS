
//int incomingByte; //USED FOR SIMPLE BUTTON PRESS TO LED
//Teensy Side of things
#include <printf.h>
#include "RF24.h"

#define CEPIN 9
#define CSPIN 10
#define PAYLOADSIZE 30 // Bytes!!

RF24 radio(CEPIN, CSPIN);

struct Data
{
  char seq[88];
  uint8_t rnd;
};

Data game = {"", 0};

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
    radio.setPALevel (RF24_PA_HIGH);
    radio.openWritingPipe (0x0000FFFF0F);
    radio.openReadingPipe (1, 0xFFFF0000F0);
    radio.setCRCLength(RF24_CRC_16);
    //radio.setPayloadSize(32);

    delay(200);
    
    radio.printDetails();
    printf("\nPayload: %d\n", radio.getPayloadSize());
    Serial.println("T E E N S Y");
    radio.startListening();
    Serial.println("Type any key to start, x to end");
    while(!Serial.available());
    if(Serial.read() == 'x')
    {
        while(1);
    }
}

int sendStructSerial(uint8_t* ptr, int size)
{
    int remaining = size;
    uint8_t confirm = 0;
    while(remaining)
    {
        if (remaining < PAYLOADSIZE)
        {
            radio.stopListening();
            radio.write(ptr, remaining);
            radio.startListening();
            while(!radio.available());
            radio.read(&confirm, 1);
              if (confirm)
                  return 1;
              else
                  return 0;
        }
        else
        {
            radio.stopListening();
            radio.write(ptr, PAYLOADSIZE);
            radio.startListening();
            while(!radio.available());
            radio.read(&confirm, 1);
            if (confirm)
            {
                remaining -= PAYLOADSIZE;
                ptr += PAYLOADSIZE;
            }
            else
                return 0;
        }
        confirm = 0;
    }
    return 1;
}


void makeString ( char *str, uint8_t *num)
{
  if (str[*num] != '\0' || *num >= 99)
  {
    *num = 0;
    str[0] = '\0';
  }
      
    int r = random(1, 4);
    switch (r) 
    {
        case 1:
        {
            str[*num] = 'R';
            break;
        }
        case 2:
        {
            str[*num] = 'G';
            break;
        }
        case 3:
        {
            str[*num] = 'Y';
            break;
        }
    }

    (*num)++;
    str[*num] = '\0';
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

    //BEGINNING OF GAME RUN
    if(game.rnd == 0)
    {
      char welcome[] = "rgyrgyygrygrrgyrgy";
      int i = 0;
        while (welcome[i] != '\0')
        {
          lightup(welcome[i++]);
          delay(25);
        }
        delay(1000);
    }

    makeString (game.seq, &game.rnd); //Returns the string of LED pattern for next round
    
    int i = 0;
    while (i < game.rnd)
        lightup (game.seq[i++]);

    //int j = 0;
    /*
    while (j <= 100)
    {
      Serial1.digitalWrite(game.seq[j++]);
    }
    */

    uint8_t* ptr = (uint8_t*)&game;

    while(!sendStructSerial(ptr, sizeof(Data)));
    char val;
    while(!radio.available());
    
    radio.read(&val, 1);
    lightup (val);

    if (val == 'R')
    {
      game.seq[0] = '\0';
      game.rnd = 0;
    }
}
    /***********************************************
    * CODE FOR TURNING ON LIGHTS BASED ON PRO-MINI
    * BUTTON INPUTS
    * 
    * 
    // see if there's incoming serial data:
    if (Serial1.available() > 0)
    {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial1.read();
    if (incomingByte == 'R') {
      digitalWrite(14, !digitalRead(14));
    }
    if (incomingByte == 'G') {
      digitalWrite(15, !digitalRead(15));
    }
    if (incomingByte == 'Y') {
      digitalWrite(16, !digitalRead(16));
    }
    }
    * 
    *************************************************/
    




















