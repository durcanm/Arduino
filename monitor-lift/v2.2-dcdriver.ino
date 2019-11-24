/*
 * ver 2.2a 23-11-2019
 * mdurcan
 * arduino mega
 *
 */


// settings
int speedLid                = 255;
int speedZ                  = 255;
int speedNeck               = 255;
bool directionUP            = true; // initial direction. true:UP false:DOWN

// button
int pin_buttonStart         = 22;
// led
int pin_led                 = 24;
// stopper
int pin_stopperLid          = 26;
int pin_stopperZ_up         = 28;
int pin_stopperZ_down       = 30;
int pin_stopperNeck         = 32;
// driver-A
int pin_A_ENA               = 10; // PWM
int pin_A_ENB               = 11; // PWM
int pin_A_IN1               = 34;
int pin_A_IN2               = 36;
int pin_A_IN3               = 38;
int pin_A_IN4               = 40;
// driver-B
int pin_B_ENA               = 12; // PWM
//int pin_B_ENB               = 13; // PWM
int pin_B_IN1               = 42;
int pin_B_IN2               = 44;
//int pin_B_IN3               = 46;
//int pin_B_IN4               = 48;


void setup()
{
    // button
    pinMode(pin_buttonStart, INPUT);
    // led
    pinMode(pin_led, OUTPUT);
    digitalWrite(pin_led, LOW);
    // stopper
    pinMode(pin_stopperLid, INPUT);
    pinMode(pin_stopperZ_up, INPUT);
    pinMode(pin_stopperZ_down, INPUT);
    pinMode(pin_stopperNeck, INPUT);
    // motor-driver
    pinMode(pin_A_ENA, OUTPUT);
    pinMode(pin_A_ENB, OUTPUT);
    pinMode(pin_A_IN1, OUTPUT);    digitalWrite(pin_A_IN1, LOW);
    pinMode(pin_A_IN2, OUTPUT);    digitalWrite(pin_A_IN2, LOW);
    pinMode(pin_A_IN3, OUTPUT);    digitalWrite(pin_A_IN3, LOW);
    pinMode(pin_A_IN4, OUTPUT);    digitalWrite(pin_A_IN4, LOW);
    pinMode(pin_B_ENA, OUTPUT);
    //pinMode(pin_B_ENB, OUTPUT);
    pinMode(pin_B_IN1, OUTPUT);    digitalWrite(pin_B_IN1, LOW);
    pinMode(pin_B_IN2, OUTPUT);    digitalWrite(pin_B_IN2, LOW);
    //pinMode(pin_B_IN3, OUTPUT);    digitalWrite(pin_B_IN3, LOW);
    //pinMode(pin_B_IN4, OUTPUT);    digitalWrite(pin_B_IN4, LOW);

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
            driveLid(); delay(500);
            driveZ(); delay(500);
            driveNeck();
        }
        else
        {
            Serial.println("direction: DOWN");
            driveNeck(); delay(500);
            driveZ(); delay(500);
            driveLid();
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

void driveLid()
{
    analogWrite(pin_A_ENA, speedLid);

    while( !isStopperHit_Lid() )
    {
        if( directionUP )
        {
            digitalWrite(pin_A_IN1, HIGH);
            digitalWrite(pin_A_IN2, LOW);
        }
        else
        {
            digitalWrite(pin_A_IN1, LOW);
            digitalWrite(pin_A_IN2, HIGH);
        }
    }

    // turn-off
    analogWrite(pin_A_ENA, 0);
    digitalWrite(pin_A_IN1, LOW);
    digitalWrite(pin_A_IN2, LOW);
}

void driveZ()
{
    analogWrite(pin_A_ENB, speedZ);
    
    while( !isStopperHit_Z() )
    {
        if( directionUP )
        {
            digitalWrite(pin_A_IN3, HIGH);
            digitalWrite(pin_A_IN4, LOW);
        }
        else
        {
             digitalWrite(pin_A_IN3, LOW);
             digitalWrite(pin_A_IN4, HIGH);
        }
    }

    // turn-off
    analogWrite(pin_A_ENB, 0);
    digitalWrite(pin_A_IN3, LOW);
    digitalWrite(pin_A_IN4, LOW);
}

void driveNeck()
{
    analogWrite(pin_B_ENA, speedNeck);

    while( !isStopperHit_Neck() )
    {
        if( directionUP )
        {
            digitalWrite(pin_B_IN1, HIGH);
            digitalWrite(pin_B_IN2, LOW);
        }
        else
        {
            digitalWrite(pin_B_IN1, LOW);
            digitalWrite(pin_B_IN2, HIGH);
        }
    }

    // turn-off
    analogWrite(pin_B_ENA, 0);
    digitalWrite(pin_B_IN1, LOW);
    digitalWrite(pin_B_IN2, LOW);
}

void setNextDirection()
{
    directionUP = !directionUP;
}




void DEBUG()
{
    // Serial.println((String)"Var 1:" + pin_buttonUpDown + " Var 2:" + pin_stopperLid + " Var 3:" + pin_stopperZ_up);

    int _speed = 200; // 0:min 255:max
    int _delay = 5000;
    
    analogWrite(pin_A_ENA, 0);
    analogWrite(pin_A_ENB, 0);
    analogWrite(pin_B_ENA, 0);

    digitalWrite(pin_A_IN1, LOW);
    digitalWrite(pin_A_IN2, LOW);
    digitalWrite(pin_A_IN3, LOW);
    digitalWrite(pin_A_IN4, LOW);
    digitalWrite(pin_B_IN1, LOW);
    digitalWrite(pin_B_IN2, LOW);
    //digitalWrite(pin_B_IN3, LOW);
    //digitalWrite(pin_B_IN4, LOW);

    Serial.println("TEST start in 1 sec...");
    
    delay(1000);

    Serial.println(" motor-1 cw");
    analogWrite(pin_A_ENA, _speed);
    digitalWrite(pin_A_IN1, HIGH);
    delay(_delay);
    
    Serial.println(" motor-1 anti-cw");
    digitalWrite(pin_A_IN1, LOW);
    digitalWrite(pin_A_IN2, HIGH);
    delay(_delay);

    Serial.println(" motor-1 off");
    analogWrite(pin_A_ENA, 0);
    digitalWrite(pin_A_IN1, LOW);
    digitalWrite(pin_A_IN2, LOW);
    delay(_delay);

    Serial.println(" motor-2 cw");
    analogWrite(pin_A_ENB, _speed);
    digitalWrite(pin_A_IN3, HIGH);
    delay(_delay);
    
    Serial.println(" motor-2 anti-cw");
    digitalWrite(pin_A_IN3, LOW);
    digitalWrite(pin_A_IN4, HIGH);
    delay(_delay);

    Serial.println(" motor-2 off");
    analogWrite(pin_A_ENA, 0);
    digitalWrite(pin_A_IN3, LOW);
    digitalWrite(pin_A_IN4, LOW);
    delay(_delay);

    Serial.println(" motor-3 cw");
    analogWrite(pin_B_ENA, _speed);
    digitalWrite(pin_B_IN1, HIGH);
    delay(_delay);
    
    Serial.println(" motor-3 anti-cw");
    digitalWrite(pin_B_IN1, LOW);
    digitalWrite(pin_B_IN2, HIGH);
    delay(_delay);

    Serial.println(" motor-3 off");
    analogWrite(pin_B_ENA, 0);
    digitalWrite(pin_B_IN1, LOW);
    digitalWrite(pin_B_IN2, LOW);
    delay(_delay);
}
