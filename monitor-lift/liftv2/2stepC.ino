/*
 * ver 2.0c 23-09-2018
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
int pin_stopperUpRight      = 40;
int pin_stopperUpLeft       = 46;
int pin_stopperDownRight    = 38;
int pin_stopperDownLeft     = 44;

// settings
int stepperDelay            = 700;
bool clockwise              = true;



void setup() {
    Serial.begin(9600);

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

        delay(500);
    }
}

bool readUpDownButton()
{
    return digitalRead(pin_buttonUpDown);
}

void setStepperDirection()
{
    digitalWrite(pin_K_DIR, clockwise?LOW:HIGH);
    digitalWrite(pin_L_DIR, clockwise?LOW:HIGH);

    if (clockwise) { clockwise=false; } else { clockwise=true; }
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
    digitalWrite(pin_K_ENABLE, LOW);
    digitalWrite(pin_L_ENABLE, LOW);

    while ( !isStop() )
    {
        digitalWrite(pin_K_STEP, HIGH);
        delayMicroseconds(5);
        digitalWrite(pin_L_STEP, HIGH);
        delayMicroseconds(stepperDelay);

        digitalWrite(pin_K_STEP, LOW);
        delayMicroseconds(5);
        digitalWrite(pin_L_STEP, LOW);
        delayMicroseconds(stepperDelay);
    }
}

void stopStepper()
{
    digitalWrite(pin_K_ENABLE, HIGH);
    digitalWrite(pin_K_DIR, LOW);
    digitalWrite(pin_K_STEP, LOW);

    digitalWrite(pin_L_ENABLE, HIGH);
    digitalWrite(pin_L_DIR, LOW);
    digitalWrite(pin_L_STEP, LOW);   
}

bool isStop()
{
    if ( digitalRead(pin_stopperUpRight) )
    {
        Serial.println("stopper up-right hit...");
        return true;
    }
    else if ( digitalRead(pin_stopperUpLeft) )
    {
        Serial.println("stopper up-left hit...");
        return true;
    }
    else if ( digitalRead(pin_stopperDownRight) )
    {
        Serial.println("stopper down-right hit...");
        return true;
    }
    else if ( digitalRead(pin_stopperDownLeft) )
    {
        Serial.println("stopper down-left hit...");
        return true;
    }
    else
    {
        return false;
    }
}