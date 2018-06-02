
//int incomingByte; //USED FOR SIMPLE BUTTON PRESS TO LED

#define PAYLOADSIZE 63;
struct Data
{
	char seq[100];
	uint8_t rnd;
};

Data game = {"", 0};

void setup()
{
  // put your setup code here, to run once:
    Serial1.begin(9600);
    pinMode(14, OUTPUT); // RED
    pinMode(15, OUTPUT); // GREEN
    pinMode(16, OUTPUT); // YELLOW
}

int sendStructSerial(uint8_t* ptr, int size)
{
	int remaining = size;
	while(remaining)
	{
		if (remaining <= PAYLOADSIZE)
		{
			Serial1.write(ptr, remaining);
			while(!Serial1.available())
	    		continue;

	    	if (Serial1.read())
	    		return 1;
	    	else
	    		return 0;
		}
		else
		{
			Serial1.write(ptr, PAYLOADSIZE);
			while(!Serial1.available())
	    		continue;

	    	if (Serial1.read())
	    		remaining -= PAYLOADSIZE;
	    	else
	    		return 0;
		}
	}
	
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
    	char welcome[] = "rgyrgyygrygrrgyrgyygrygrrgyrgyygrygrrgyrgyygrygr";
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

    while(!sendStructSerial(ptr, sizeof(Data))
    	continue;

    char val = Serial1.read();
    lightup (val);

    if (val == 'R')
    {
    	game.seq[0] = '\0';
    	game.rnd = 0;
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
    
    

}





















