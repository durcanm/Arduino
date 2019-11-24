/*
 * ver 2.1a 24-11-2019
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
    int _delay = 3000;

    delay(100);

    digitalWrite(pin_relay1, HIGH); delay(_delay);
    digitalWrite(pin_relay1, LOW); delay(_delay);  

    digitalWrite(pin_relay2, HIGH); delay(_delay);
    digitalWrite(pin_relay2, LOW); delay(_delay);

    digitalWrite(pin_relay3, HIGH); delay(_delay);
    digitalWrite(pin_relay3, LOW); delay(_delay);

    digitalWrite(pin_relay4, HIGH); delay(_delay);
    digitalWrite(pin_relay4, LOW); delay(_delay);

    digitalWrite(pin_relay5, HIGH); delay(_delay);
    digitalWrite(pin_relay5, LOW); delay(_delay);

    delay(_delay);

    digitalWrite(pin_relay1, HIGH);
    digitalWrite(pin_relay2, HIGH);
    digitalWrite(pin_relay3, HIGH);
    digitalWrite(pin_relay4, HIGH);
    digitalWrite(pin_relay5, HIGH);

    delay(_delay);

    digitalWrite(pin_relay1, LOW);
    digitalWrite(pin_relay2, LOW);
    digitalWrite(pin_relay3, LOW);
    digitalWrite(pin_relay4, LOW);
    digitalWrite(pin_relay5, LOW);

    delay(_delay);  
}
