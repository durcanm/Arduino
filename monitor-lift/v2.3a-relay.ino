/*
 * ver 2.1a 24-11-2019
 * mdurcan
 * 
 */

// settings
int timeLid_A               = 10000;
int timeLid_B               = 10000;
int timeZ_A                 = 10000;
int timeZ_B                 = 10000;
int timeNeck_A              = 10000;
int timeNeck_B              = 10000;

bool directionUP            = true; // initial direction. true:UP false:DOWN

// button
int pin_buttonStart         = 2;
int pin_buttonStart2        = 3;
// led
int pin_led                 = 7;
// relay
int pin_relayMain           = 9;
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
    pinMode(pin_buttonStart2, INPUT);
    // led
    pinMode(pin_led, OUTPUT);
    digitalWrite(pin_led, LOW);
    // relay
    pinMode(pin_relayMain, OUTPUT);
    digitalWrite(pin_relayMain, LOW);
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
    //DEBUG(); return;

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

    else if ( readStart2Button() )
    {
        Serial.println("progress#2 started...");

        delay(200);

        controlLed(true); // led ON

        driveRelayMain(true); // enable Relays

        delay(200);

        if ( directionUP )
        {
            Serial.println("direction: UP");
            driveRelayLid2(); delay(500);
            driveRelayZ2(); delay(500);
            driveRelayNeck2();
        }
        else
        {
            Serial.println("direction: DOWN");
            driveRelayNeck2(); delay(500);
            driveRelayZ2(); delay(500);
            driveRelayLid2();
        }

        setNextDirection();

        delay(200);

        driveRelayMain(false); // disable Relays

        controlLed(false); // led OFF

        Serial.println("progress#2 stopped...");

        delay( 2000 );
    }
}

bool readStartButton()
{
    return digitalRead( pin_buttonStart );
}

bool readStart2Button()
{
    return digitalRead( pin_buttonStart2 );
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
    if ( digitalRead(pin_stopperZ) )
    {
        Serial.println("stopper -Z- hit...");
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

void driveRelayMain(bool isOn)
{
    if (isOn)
    { digitalWrite(pin_relayMain, HIGH); }
    else
    { digitalWrite(pin_relayMain, LOW); }
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

void driveRelayLid2()
{
    digitalWrite(pin_relayLid, HIGH);
    
    if (directionUP)
    { delay(timeLid_A); }
    else
    { delay(timeLid_B); }
    
    digitalWrite(pin_relayLid, LOW);
}

void driveRelayZ2()
{
    digitalWrite(pin_relayZ, HIGH);
    
    if (directionUP)
    { delay(timeZ_A); }
    else
    { delay(timeZ_B); }
    
    digitalWrite(pin_relayZ, LOW);
}

void driveRelayNeck2()
{
    digitalWrite(pin_relayNeck, HIGH);
    
    if (directionUP)
    { delay(timeNeck_A); }
    else
    { delay(timeNeck_B); }
    
    digitalWrite(pin_relayNeck, LOW);
}

void setNextDirection()
{
    directionUP = !directionUP;
  
    if ( directionUP )
    {
      digitalWrite(pin_relayDirection1, LOW);
      digitalWrite(pin_relayDirection2, LOW);
    }
    else
    {
      digitalWrite(pin_relayDirection1, HIGH);
      digitalWrite(pin_relayDirection2, HIGH);
    }
}




void DEBUG()
{
    int _delay = 3000;

    delay(100);

    digitalWrite(pin_relayLid, HIGH); delay(_delay);
    digitalWrite(pin_relayLid, LOW); delay(_delay);  

    digitalWrite(pin_relayZ, HIGH); delay(_delay);
    digitalWrite(pin_relayZ, LOW); delay(_delay);

    digitalWrite(pin_relayNeck, HIGH); delay(_delay);
    digitalWrite(pin_relayNeck, LOW); delay(_delay);

    digitalWrite(pin_relayDirection1, HIGH); delay(_delay);
    digitalWrite(pin_relayDirection1, LOW); delay(_delay);

    digitalWrite(pin_relayDirection2, HIGH); delay(_delay);
    digitalWrite(pin_relayDirection2, LOW); delay(_delay);

    delay(_delay);

    digitalWrite(pin_relayLid, HIGH);
    digitalWrite(pin_relayZ, HIGH);
    digitalWrite(pin_relayNeck, HIGH);
    digitalWrite(pin_relayDirection1, HIGH);
    digitalWrite(pin_relayDirection2, HIGH);

    delay(_delay);

    digitalWrite(pin_relayLid, LOW);
    digitalWrite(pin_relayZ, LOW);
    digitalWrite(pin_relayNeck, LOW);
    digitalWrite(pin_relayDirection1, LOW);
    digitalWrite(pin_relayDirection2, LOW);

    delay(_delay);  
}