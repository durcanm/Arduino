/*
 * ver 1.0a 28-09-2018
 * mdurcan
 * 
 */

// settings
int stepperDelay            = 700;  // microseconds
bool directionUP            = true; // true:UP false:DOWN

// button                   
int pin_buttonUpDown        = 52;

// led                      
int pin_led                 = 50;

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
    //DEBUG_btn();
    //DEBUG_motor();
    //DEBUG_stopper();
    //return;

    if ( readUpDownButton() )
    {
        Serial.println("progress started...");

        if( directionUP ) { Serial.println("direction: UP"); } else { Serial.println("direction: DOWN"); }

        controlLed(true); // led ON

        driveStepper();

        delayMicroseconds(3);

        stopStepper();

        setNextDirection();

        controlLed(false); // led OFF

        Serial.println("progress stopped...");

        delay( 1000 );
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

void driveStepper()
{
    // enable stepper
    digitalWrite(pin_A_ENABLE, LOW);
    digitalWrite(pin_B_ENABLE, LOW);

    while ( !isStop_A() || !isStop_B() )
    {
        if ( !isStop_A() )
        {            
            digitalWrite(pin_A_STEP, HIGH);
            delayMicroseconds(stepperDelay);

            digitalWrite(pin_A_STEP, LOW);
            delayMicroseconds(stepperDelay);
        }

        delayMicroseconds(3);

        if ( !isStop_B() )
        {
            digitalWrite(pin_B_STEP, HIGH);
            delayMicroseconds(stepperDelay);
            
            digitalWrite(pin_B_STEP, LOW);
            delayMicroseconds(stepperDelay);
        }
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
    digitalWrite(pin_A_DIR, directionUP?LOW:HIGH);
    digitalWrite(pin_B_DIR, directionUP?LOW:HIGH);

    if ( directionUP ) { directionUP = false; } else { directionUP = true; }
}

bool isStop_A()
{
    if ( !digitalRead(pin_stopper_A_up) )
    {
        Serial.println("stopper A-up hit...");
        return true;
    }
    else if ( !digitalRead(pin_stopper_A_down) )
    {
        Serial.println("stopper A-down hit...");
        return true;
    }
    else
    {
        return false;
    }
}

bool isStop_B()
{
    if ( !digitalRead(pin_stopper_B_up) )
    {
        Serial.println("stopper B-up hit...");
        return true;
    }
    else if ( !digitalRead(pin_stopper_B_down) )
    {
        Serial.println("stopper B-down hit...");
        return true;
    }
    else
    {
        return false;
    }
}





void DEBUG_btn()
{
    if( digitalRead( pin_buttonUpDown ) )   { Serial.println("buttonUpDown hit..."); }
    
    if( !digitalRead( pin_stopper_A_up ) )   { Serial.println("pin_stopper_A_up hit..."); }
    if( !digitalRead( pin_stopper_A_down ) ) { Serial.println("pin_stopper_A_down hit..."); }
    if( !digitalRead( pin_stopper_B_up ) )   { Serial.println("pin_stopper_B_up hit..."); }
    if( !digitalRead( pin_stopper_B_down ) ) { Serial.println("pin_stopper_B_down hit..."); }
}

void DEBUG_motor()
{
    digitalWrite(pin_A_ENABLE, LOW);
    digitalWrite(pin_A_DIR, directionUP?LOW:HIGH);
    delayMicroseconds(2);

    Serial.println("direction...");
    Serial.println(directionUP);

    if ( directionUP ) { directionUP=false; } else { directionUP=true; }

    for (int i=0; i<500; i++)
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
    delay(10000);
}

void DEBUG_stopper()
{
    //if ( !isStop_A() ) { Serial.println("isStop_A: false"); }
    //if ( !isStop_B() ) { Serial.println("isStop_B: false"); }

    Serial.println("isStop_A:");
    Serial.println( isStop_A() );
    Serial.println("isStop_B:");
    Serial.println( isStop_B() );
}