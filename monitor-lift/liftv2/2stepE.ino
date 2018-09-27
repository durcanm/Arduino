/*
 * ver 2.0e 27-09-2018
 * mdurcan
 * 
 */

// stepper K
int pin_K_ENABLE            = 2;
int pin_K_DIR               = 4;
int pin_K_STEP              = 3;

// stepper L                
int pin_L_ENABLE            = 5;
int pin_L_DIR               = 7;
int pin_L_STEP              = 6;

// led                      
int pin_led                 = 50;

// button                   
int pin_buttonUpDown        = 52;

// stopper                  
int pin_stopper_K_up        = 40;
int pin_stopper_K_down      = 46;
int pin_stopper_L_up        = 38;
int pin_stopper_L_down      = 44;

// settings
int stepperDelay            = 700;
bool clockwise              = true;
bool endOfMove_K            = false;
bool endOfMove_L            = false;


void setup() {

    // button
    pinMode(pin_buttonUpDown, INPUT);

    // led
    pinMode(pin_led, OUTPUT);
    digitalWrite(pin_led, LOW);

    // step K
    pinMode(pin_K_ENABLE, OUTPUT);
    pinMode(pin_K_DIR, OUTPUT);
    pinMode(pin_K_STEP, OUTPUT);

    digitalWrite(pin_K_ENABLE, HIGH); // LOW:enable,HIGH:disable
    digitalWrite(pin_K_DIR, LOW); // LOW:clockwise, HIGH:anti-clockwise
    digitalWrite(pin_K_STEP, LOW);

    // step L
    pinMode(pin_L_ENABLE, OUTPUT);
    pinMode(pin_L_DIR, OUTPUT);
    pinMode(pin_L_STEP, OUTPUT);

    digitalWrite(pin_L_ENABLE, HIGH);
    digitalWrite(pin_L_DIR, LOW);
    digitalWrite(pin_L_STEP, LOW);

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
    digitalWrite(pin_K_DIR, clockwise?LOW:HIGH);
    digitalWrite(pin_L_DIR, clockwise?LOW:HIGH);

    if (clockwise) { clockwise=false; } else { clockwise=true; }

    endOfMove_K = false;
    endOfMove_L = false;
}

void driveStepper()
{
    // enable stepper
    digitalWrite(pin_K_ENABLE, LOW);
    digitalWrite(pin_L_ENABLE, LOW);

    while ( !isStop_K() || !isStop_L() )
    {
        if( !isStop_K() )
        {
            digitalWrite(pin_K_STEP, HIGH);
            delayMicroseconds(5);

            digitalWrite(pin_K_STEP, LOW);
            delayMicroseconds(5);
        }

        if( !isStop_L() )
        {
            digitalWrite(pin_L_STEP, HIGH);
            delayMicroseconds(stepperDelay);
            
            digitalWrite(pin_L_STEP, LOW);
            delayMicroseconds(stepperDelay);
        }
    }
}

void stopStepper()
{
    digitalWrite(pin_K_ENABLE, HIGH);
    //digitalWrite(pin_K_DIR, LOW);
    digitalWrite(pin_K_STEP, LOW);

    digitalWrite(pin_L_ENABLE, HIGH);
    //digitalWrite(pin_L_DIR, LOW);
    digitalWrite(pin_L_STEP, LOW);   
}

bool isStop_K()
{
    if ( !digitalRead(pin_stopper_K_up) )
    {
        Serial.println("stopper K-up hit...");
        endOfMove_K=true;
        return true;
    }
    else if ( !digitalRead(pin_stopper_K_down) )
    {
        Serial.println("stopper K-down hit...");
        endOfMove_K=true;
        return true;
    }
    else
    {
        return false;
    }
}

bool isStop_L()
{
    if ( !digitalRead(pin_stopper_L_up) )
    {
        Serial.println("stopper L-up hit...");
        endOfMove_L=true;
        return true;
    }
    else if ( !digitalRead(pin_stopper_L_down) )
    {
        Serial.println("stopper L-down hit...");
        endOfMove_L=true;
        return true;
    }
    else
    {
        return false;
    }
}