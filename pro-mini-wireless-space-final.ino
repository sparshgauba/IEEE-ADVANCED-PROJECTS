//Arduino Side of things
#include <printf.h>
#include <SoftwareSerial.h>
#include "RF24.h"

#define CEPIN 9
#define CSPIN 10
#define PAYLOADSIZE 32 // Bytes!!

RF24 radio(CEPIN, CSPIN);

SoftwareSerial mySerial (3,4);// (Rx, Tx)
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
	char seq[88];
	uint8_t rnd;
};


//Data game = {"", 0};

char game[89] = "";
Data* gameptr = (Data*)&game;

void setup()
{
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
    radio.setCRCLength(RF24_CRC_16);
    //radio.setPayloadSize(16);
  
    delay(200);
  
    radio.printDetails();
    printf("\n%d\n", radio.getPayloadSize());
    Serial.println("P R O    M I N I");
    radio.startListening();
}

void getStructData(char* ptr, int size)
{
	int remaining = size;
	uint8_t confirm = 1;
	while(remaining > 0)
	{
		if(remaining < PAYLOADSIZE)
		{
			while(!radio.available());
			radio.read(ptr, remaining);
			radio.stopListening();
			radio.write(&confirm, 1);
			radio.startListening();
			remaining = 0;
		}
		else
		{
			while(!radio.available());
			radio.read(ptr, PAYLOADSIZE);
			radio.stopListening();
			radio.write(&confirm, 1);
			radio.startListening();
			remaining -= PAYLOADSIZE;
            ptr += PAYLOADSIZE;
		}
	}
	return;
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
	//Serial.println ("Beginning of SHIT THE GAME");
	getStructData(game, sizeof(Data));

	/*while (ptr->seq[i] != '\0')
	{
		Serial.print(ptr->seq[i++]);
	}*/

	int j = 0;
    while (gameptr->seq[j] != '\0')
    {
    	if (inputButton() == gameptr->seq[j])
    		j++;
    	else
    		break;
    }

    if (gameptr->seq[j] == '\0')
    {
    	char send = 'G';
    	radio.stopListening();
    	radio.write(&send, 1);
    	radio.startListening();
    }
    else
    {
    	char send = 'R';
    	radio.stopListening();
    	radio.write(&send, 1);
    	radio.startListening();
    }
	
  }

















