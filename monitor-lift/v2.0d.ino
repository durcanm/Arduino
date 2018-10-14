/*
 * ver 2.0d 14-10-2018
 * mdurcan
 * 
 */

// user settings
int stepperDelay            = 300;  // microseconds

// app settings
bool directionUP            = true; // initial direction. true:UP false:DOWN

// button                   
int pin_buttonUpDown        = 52;

// led                      
int pin_led                 = 50;

// relay
int pin_relay               = 51;

// stopper                  
int pin_stopper_A_up        = 46;
int pin_stopper_A_down      = 44;
int pin_stopper_B_up        = 40;
int pin_stopper_B_down      = 38;

// stepper A
int pin_A_ENABLE            = 5;
int pin_A_DIR               = 7;
int pin_A_STEP              = 6;

// stepper B                
int pin_B_ENABLE            = 2;
int pin_B_DIR               = 4;
int pin_B_STEP              = 3;


void setup()
{
    // button
    pinMode(pin_buttonUpDown, INPUT);

    // led
    pinMode(pin_led, OUTPUT);
    digitalWrite(pin_led, LOW);

    // relay
    pinMode(pin_relay, OUTPUT);
    digitalWrite(pin_relay, LOW);

    // stopper
    pinMode(pin_stopper_A_up, INPUT);
    pinMode(pin_stopper_A_down, INPUT);
    pinMode(pin_stopper_B_up, INPUT);
    pinMode(pin_stopper_B_down, INPUT);

    // step A
    pinMode(pin_A_ENABLE, OUTPUT);
    pinMode(pin_A_DIR, OUTPUT);
    pinMode(pin_A_STEP, OUTPUT);

    digitalWrite(pin_A_ENABLE, HIGH); // LOW:enable, HIGH:disable
    digitalWrite(pin_A_DIR, LOW); // LOW:UP, HIGH:DOWN
    digitalWrite(pin_A_STEP, LOW);

    // step B
    pinMode(pin_B_ENABLE, OUTPUT);
    pinMode(pin_B_DIR, OUTPUT);
    pinMode(pin_B_STEP, OUTPUT);

    digitalWrite(pin_B_ENABLE, HIGH);
    digitalWrite(pin_B_DIR, LOW);
    digitalWrite(pin_B_STEP, LOW);

    Serial.begin(9600);
    Serial.println("setup completed...");
}

void loop()
{
    //DEBUG_buton();
    //DEBUG_direction();
    //DEBUG_movement();
    //DEBUG_motor();
    //return;
    

    if ( readUpDownButton() )
    {
        Serial.println("progress started...");

        if( directionUP )  { Serial.println("direction: UP"); }
        if( !directionUP ) { Serial.println("direction: DOWN"); }

        controlLed(true); // led ON

        driveStepper();

        delayMicroseconds(3);

        stopStepper();

        setNextDirection();

        controlRelay();

        controlLed(false); // led OFF

        Serial.println("progress stopped...");

        delay( 3000 );
    }
}

bool readUpDownButton()
{
    return digitalRead( pin_buttonUpDown );
}

void controlLed(bool isOn)
{
    if (isOn)
    {
        digitalWrite(pin_led, HIGH);
    }
    else
    {
        digitalWrite(pin_led, LOW);
    }
}

void controlRelay()
{
    if (directionUP)
    {
        digitalWrite(pin_relay, HIGH);
    }
    else
    {
        digitalWrite(pin_relay, LOW);
    }
}

void driveStepper()
{
    // enable stepper
    digitalWrite(pin_A_ENABLE, LOW);
    digitalWrite(pin_B_ENABLE, LOW);

    //Serial.println("/// while start ///");

    while ( !endOfMove_A() || !endOfMove_B() )
    {
        if( readUpDownButton() ) { return; }
        
        if ( !endOfMove_A() )
        {
            digitalWrite(pin_A_STEP, HIGH);
            delayMicroseconds(stepperDelay);

            digitalWrite(pin_A_STEP, LOW);
            delayMicroseconds(stepperDelay);

            // Serial.print("motor A ");
            // Serial.println(directionUP);
            // delay(250);
        }

        delayMicroseconds(3);

        if ( !endOfMove_B() )
        {
            digitalWrite(pin_B_STEP, HIGH);
            delayMicroseconds(stepperDelay);

            digitalWrite(pin_B_STEP, LOW);
            delayMicroseconds(stepperDelay);

            // Serial.print("motor B ");
            // Serial.println(directionUP);
            // delay(250);
        }
    }

    //Serial.println("*** while end ***");
}

bool endOfMove_A()
{
    if ( directionUP )
    {
        return isStopperHit_A_up();
    }

    if ( !directionUP )
    {
        return isStopperHit_A_down();
    }
}

bool endOfMove_B()
{
    if ( directionUP )
    {
        return isStopperHit_B_up();
    }

    if ( !directionUP )
    {
        return isStopperHit_B_down();
    }
}

void stopStepper()
{
    digitalWrite(pin_A_ENABLE, HIGH);
    digitalWrite(pin_A_STEP, LOW);

    digitalWrite(pin_B_ENABLE, HIGH);
    digitalWrite(pin_B_STEP, LOW);   
}

void setNextDirection()
{
    if ( directionUP ) { directionUP = false; } else { directionUP = true; }
    
    digitalWrite(pin_A_DIR, directionUP?LOW:HIGH);
    digitalWrite(pin_B_DIR, directionUP?LOW:HIGH);

    //Serial.print("next direction: ");
    //Serial.println(directionUP);
}

bool isStopperHit_A_up()
{
    if ( !digitalRead(pin_stopper_A_up) )
    {
        Serial.println("stopper A-up hit...");
        return true;
    }
    else
    {
        return false;
    }
}

bool isStopperHit_A_down()
{
    if ( !digitalRead(pin_stopper_A_down) )
    {
        Serial.println("stopper A-down hit...");
        return true;
    }
    else
    {
        return false;
    }
}

bool isStopperHit_B_up()
{
    if ( !digitalRead(pin_stopper_B_up) )
    {
        Serial.println("stopper B-up hit...");
        return true;
    }
    else
    {
        return false;
    }
}

bool isStopperHit_B_down()
{
    if ( !digitalRead(pin_stopper_B_down) )
    {
        Serial.println("stopper B-down hit...");
        return true;
    }
    else
    {
        return false;
    }
}








void DEBUG_buton()
{
    // HIGH is true
    if ( digitalRead( pin_buttonUpDown ) )       { Serial.println("buttonUpDown hit..."); }
    
    // stoppers are connected inversely so LOW is true!
    if ( !digitalRead( pin_stopper_A_up ) )      { Serial.println("stopper_A_up hit..."); }
    if ( !digitalRead( pin_stopper_A_down ) )    { Serial.println("stopper_A_down hit..."); }
    if ( !digitalRead( pin_stopper_B_up ) )      { Serial.println("stopper_B_up hit..."); }
    if ( !digitalRead( pin_stopper_B_down ) )    { Serial.println("stopper_B_down hit..."); }

    delay(300);
}

void DEBUG_direction()
{
    Serial.print("simdiki yön: ");

    if (directionUP)
    {
        Serial.println("yukari");
    }
    if (!directionUP)
    {
        Serial.println("asagi");
    }
    if ( readUpDownButton() )
    {        
        setNextDirection();
    }

    delay(300);
}

void DEBUG_movement()
{
    Serial.print("directionUP: ");
    Serial.println(directionUP);

    Serial.print("endOfMove_A: ");
    Serial.println( endOfMove_A() );

    Serial.print("endOfMove_B: ");
    Serial.println( endOfMove_B() );   

    setNextDirection();

    delay(1200);
}

void DEBUG_motor()
{
    digitalWrite(pin_A_ENABLE, LOW);
    digitalWrite(pin_A_DIR, directionUP?LOW:HIGH);
    delayMicroseconds(2);

    Serial.print("direction: ");
    Serial.println(directionUP);

    if ( directionUP ) { directionUP=false; } else { directionUP=true; }

    for (int i=0; i<1000; i++)
    {
        digitalWrite(pin_A_STEP, HIGH);
        delayMicroseconds(700);
        digitalWrite(pin_A_STEP, LOW);
        delayMicroseconds(700);
    }

    digitalWrite(pin_A_ENABLE, HIGH);
    digitalWrite(pin_A_DIR, LOW);
    digitalWrite(pin_A_STEP, LOW);

    Serial.println("stop...");
    delay(5000);
}
