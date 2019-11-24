/*
 * ver 2.1 16-11-2019
 * mdurcan
 * 
 */

// settings
bool directionUP            = true; // initial direction. true:UP false:DOWN

// button
int pin_buttonStart         = 2;
// led
int pin_led                 = 7;
// relay
int pin_relayLid            = 12;
int pin_relayZ              = 11;
int pin_relayNeck           = 10;
int pin_relayDirection1     = 13;
int pin_relayDirection2     = 8;
// stopper
int pin_stopperLid          = 5;
int pin_stopperZ            = 4;
int pin_stopperNeck         = 6;


void setup()
{
    // button
    pinMode(pin_buttonStart, INPUT);
    // led
    pinMode(pin_led, OUTPUT);
    digitalWrite(pin_led, LOW);
    // relay
    pinMode(pin_relayLid, OUTPUT);
    digitalWrite(pin_relayLid, LOW);
    pinMode(pin_relayZ, OUTPUT);
    digitalWrite(pin_relayZ, LOW);
    pinMode(pin_relayNeck, OUTPUT);
    digitalWrite(pin_relayNeck, LOW);
    pinMode(pin_relayDirection1, OUTPUT);
    digitalWrite(pin_relayDirection1, LOW);
    pinMode(pin_relayDirection2, OUTPUT);
    digitalWrite(pin_relayDirection2, LOW);
    // stopper
    pinMode(pin_stopperLid, INPUT);
    pinMode(pin_stopperZ, INPUT);
    pinMode(pin_stopperNeck, INPUT);

    Serial.begin(9600);
    Serial.println("setup completed...");
}

void loop()
{
    DEBUG(); return;

    if ( readStartButton() )
    {
        Serial.println("progress started...");

        delay(200);

        controlLed(true); // led ON

        delay(200);

        if ( directionUP )
        {
            Serial.println("direction: UP");
            driveRelayLid(); delay(500);
            driveRelayZ(); delay(500);
            driveRelayNeck();
        }
        else
        {
            Serial.println("direction: DOWN");
            driveRelayNeck(); delay(500);
            driveRelayZ(); delay(500);
            driveRelayLid();
        }

        setNextDirection();

        delay(200);

        controlLed(false); // led OFF        

        Serial.println("progress stopped...");

        delay( 2000 );
    }
}

bool readStartButton()
{
    return digitalRead( pin_buttonStart );
}

void controlLed(bool isOn)
{
    if (isOn)
    { digitalWrite(pin_led, HIGH); }
    else
    { digitalWrite(pin_led, LOW); }
}

bool isStopperHit_Lid()
{
    if ( digitalRead(pin_stopperLid) )
    {
        Serial.println("stopper -Lid- hit...");
        return true;
    }
    else
    { return false; }
}

bool isStopperHit_Z()
{
    if ( directionUP && digitalRead(pin_stopperZ_up) )
    {
        Serial.println("stopper -Z(up)- hit...");
        return true;
    }
    else if ( !directionUP && digitalRead(pin_stopperZ_down) )
    {
        Serial.println("stopper -Z(down)- hit...");
        return true;
    }
    else
    { return false; }
}

bool isStopperHit_Neck()
{
    if ( digitalRead(pin_stopperNeck) )
    {
        Serial.println("stopper -Neck(up)- hit...");
        return true;
    }
    else
    { return false; }
}

void driveRelayLid()
{
    while( !isStopperHit_Lid() )
    { digitalWrite(pin_relayLid, HIGH); }

    digitalWrite(pin_relayLid, LOW);
}

void driveRelayZ()
{
    while( !isStopperHit_Z() )
    { digitalWrite(pin_relayZ, HIGH); }

    digitalWrite(pin_relayZ, LOW);
}

void driveRelayNeck()
{
    while( !isStopperHit_Neck() )
    { digitalWrite(pin_relayNeck, HIGH); }

    digitalWrite(pin_relayNeck, LOW);
}

void setNextDirection()
{
    directionUP = !directionUP;
  
    if ( directionUP )
    { digitalWrite(pin_relayDirection, LOW); }
    else
    { digitalWrite(pin_relayDirection, HIGH); }
}




void DEBUG()
{
    //Serial.println((String)"Var 1:" + pin_buttonUpDown + " Var 2:" + pin_stopperLid + " Var 3:" + pin_stopperZ_up);
  
  /*
    delay(100);    

    Serial.print( digitalRead( pin_buttonStart ) );
    Serial.print( digitalRead( pin_stopperLid ) );
    Serial.print( digitalRead( pin_stopperZ_up ) );
    Serial.print( digitalRead( pin_stopperZ_down ) );
    Serial.print( digitalRead( pin_stopperNeck ) );
    Serial.println( "- - - - -" );
  */
  /*
    digitalWrite(pin_relayDirection, LOW);

    digitalWrite(pin_relayZ, HIGH);
    digitalWrite(pin_relayNeck, HIGH);
    digitalWrite(pin_relayLid, HIGH); 

    delay(1000);  

    digitalWrite(pin_relayZ, LOW);
    digitalWrite(pin_relayNeck, LOW);
    digitalWrite(pin_relayLid, LOW); 

    delay(200);

    digitalWrite(pin_relayDirection, HIGH);

    digitalWrite(pin_relayZ, HIGH);
    digitalWrite(pin_relayNeck, HIGH);
    digitalWrite(pin_relayLid, HIGH); 

    delay(1000);
  */
  
    digitalWrite(pin_relayDirection, LOW); delay(30);
	
    digitalWrite(pin_relayLid, HIGH); delay(30);
    digitalWrite(pin_relayZ, HIGH); delay(30);
    digitalWrite(pin_relayNeck, HIGH); delay(30);
    
    delay(500);

    digitalWrite(pin_relayDirection, HIGH); delay(30);
    delay(500);

}
