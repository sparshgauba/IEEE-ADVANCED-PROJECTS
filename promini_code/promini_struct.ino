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

struct Data
{
	char seq[100];
	uint8_t rnd;
};


//Data game = {"", 0};

char game[101] = "";
Data* ptr = (Data*)&game;

void setup()
{
	// initialize serial communication:
	Serial.begin(9600);
	mySerial.begin(9600);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(7, INPUT);
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
	while (!mySerial.available())
		continue;

	if (mySerial.readBytes(game, 63) == 63)
	{
		mySerial.write(1);
	}

	while (!mySerial.available())
		continue;

	if (mySerial.readBytes((game + 63), 38) == 38)
	{
		mySerial.write(1);
	}


	/*while (ptr->seq[i] != '\0')
	{
		Serial.print(ptr->seq[i++]);
	}*/

	int j = 0;
    while (ptr->seq[j] != '\0')
    {
    	if (inputButton() == ptr->seq[j])
    		j++;
    	else
    		break;
    }

    if (ptr->seq[j] == '\0')
    	mySerial.write('G');
    else
    	mySerial.write('R');
	
  }

















