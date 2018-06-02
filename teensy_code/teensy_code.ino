
//int incomingByte; //USED FOR SIMPLE BUTTON PRESS TO LED
String game = "";
int rnd = 0;
void setup()
{
  // put your setup code here, to run once:
    Serial1.begin(9600);
    pinMode(14, OUTPUT); // RED
    pinMode(15, OUTPUT); // GREEN
    pinMode(16, OUTPUT); // YELLOW
}

String makeString (String str, int num)
{
    if (str.length() >= num)
      str = "";
      
    int r = random(1, 4);
    switch (r) 
    {
        case 1:
        {
            str += 'R';
            break;
        }
        case 2:
        {
            str += 'G';
            break;
        }
        case 3:
        {
            str += 'Y';
            break;
        }
    }
    
    return str;
}

void lightup (char led)
{
    if (led == 'R')
    {
        digitalWrite(14, HIGH);
        delay(500);
        digitalWrite(14, LOW);
        delay(250);
    }
    if (led == 'G')
    {
        digitalWrite(15, HIGH);
        delay(500);
        digitalWrite(15, LOW);
        delay(250);
    }
    if (led == 'Y')
    {
        digitalWrite(16, HIGH);
        delay(500);
        digitalWrite(16, LOW);
        delay(250);
    }
}

void loop()
{
    //BEGINNING OF GAME RUN
    if(rnd == 0)
    {
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(14, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(14, LOW);
        delay(25);
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(14, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(14, LOW);
        delay(25);
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(14, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(14, LOW);
        delay(25);
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(14, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(16, HIGH);
        delay(25);
        digitalWrite(15, HIGH);
        delay(25);
        digitalWrite(14, HIGH);
        delay(25);
        digitalWrite(16, LOW);
        delay(25);
        digitalWrite(15, LOW);
        delay(25);
        digitalWrite(14, LOW);
        delay(1000);
    }
    // contains the string of LED lighting order
    rnd++;
    game = makeString (game, rnd); //Returns the string of LED pattern for next round
    
    
    int i = 0;
    while (i < game.length())
        lightup (game[i++]);
        
    int j = 0;
    while (j < game.length())
    {
        if (Serial1.available() > 0)
        {
            // read the oldest byte in the serial buffer:
            if (Serial1.read() == game[j])
                j++;
            else
            {
                digitalWrite(14, HIGH);
                delay(1000);
                digitalWrite(14, LOW);
                delay(1000);
                rnd = 0;
                break;
            }
        }
        else
            continue;
    }
    
    if (j == game.length())
    {
        digitalWrite(15, HIGH);
        delay(1000);
        digitalWrite(15, LOW);
        delay(1000);
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





















