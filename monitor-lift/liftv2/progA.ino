/*
 * ver 2.0e 27-09-2018
 * mdurcan
 * 
 */

// stepper A
int pin_A_ENABLE            = 2;
int pin_A_DIR               = 4;
int pin_A_STEP              = 3;

// stepper B                
int pin_B_ENABLE            = 5;
int pin_B_DIR               = 7;
int pin_B_STEP              = 6;

// led                      
int pin_led                 = 50;

// button                   
int pin_buttonUpDown        = 52;

// stopper                  
int pin_stopper_A_up        = 40;
int pin_stopper_A_down      = 46;
int pin_stopper_B_up        = 38;
int pin_stopper_B_down      = 44;

// settings
int stepperDelay            = 700;
bool clockwise              = true;
bool endOfMove_A            = false;
bool endOfMove_B            = false;


void setup() {

    // button
    pinMode(pin_buttonUpDown, INPUT);

    // led
    pinMode(pin_led, OUTPUT);
    digitalWrite(pin_led, LOW);

    // step A
    pinMode(pin_A_ENABLE, OUTPUT);
    pinMode(pin_A_DIR, OUTPUT);
    pinMode(pin_A_STEP, OUTPUT);

    digitalWrite(pin_A_ENABLE, HIGH); // LOW:enable,HIGH:disable
    digitalWrite(pin_A_DIR, LOW); // LOW:clockwise, HIGH:anti-clockwise
    digitalWrite(pin_A_STEP, LOW);

    // step B
    pinMode(pin_B_ENABLE, OUTPUT);
    pinMode(pin_B_DIR, OUTPUT);
    pinMode(pin_B_STEP, OUTPUT);

    digitalWrite(pin_B_ENABLE, HIGH);
    digitalWrite(pin_B_DIR, LOW);
    digitalWrite(pin_B_STEP, LOW);

    Serial.begin(9600);
    Serial.println("settings applied...");
}

void loop()
{
    if ( readUpDownButton() )
    {
        Serial.println("start...");

        controlLed(true); // led ON

        setStepperDirection();

        driveStepper();

        delayMicroseconds(2);

        stopStepper();

        controlLed(false); // led OFF

        Serial.println("stop...");

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

void setStepperDirection()
{
    digitalWrite(pin_A_DIR, clockwise?LOW:HIGH);
    digitalWrite(pin_B_DIR, clockwise?LOW:HIGH);

    if (clockwise) { clockwise=false; } else { clockwise=true; }

    endOfMove_A = false;
    endOfMove_B = false;
}

void driveStepper()
{
    // enable stepper
    digitalWrite(pin_A_ENABLE, LOW);
    digitalWrite(pin_B_ENABLE, LOW);

    while ( !isStop_A() || !isStop_B() )
    {
        if( !isStop_A() )
        {
            digitalWrite(pin_A_STEP, HIGH);
            delayMicroseconds(5);

            digitalWrite(pin_A_STEP, LOW);
            delayMicroseconds(5);
        }

        if( !isStop_B() )
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
    //digitalWrite(pin_A_DIR, LOW);
    digitalWrite(pin_A_STEP, LOW);

    digitalWrite(pin_B_ENABLE, HIGH);
    //digitalWrite(pin_B_DIR, LOW);
    digitalWrite(pin_B_STEP, LOW);   
}

bool isStop_A()
{
    if ( !digitalRead(pin_stopper_A_up) )
    {
        Serial.println("stopper A-up hit...");
        endOfMove_A=true;
        return true;
    }
    else if ( !digitalRead(pin_stopper_A_down) )
    {
        Serial.println("stopper A-down hit...");
        endOfMove_A=true;
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
        endOfMove_B=true;
        return true;
    }
    else if ( !digitalRead(pin_stopper_B_down) )
    {
        Serial.println("stopper B-down hit...");
        endOfMove_B=true;
        return true;
    }
    else
    {
        return false;
    }
}