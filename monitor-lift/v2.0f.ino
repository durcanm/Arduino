/*
 * ver 1.0  28-10-2017
 * ver 2.0b 29-09-2018
 * ver 2.0e 17-10-2018
 * ver 2.0f 03-02-2019
 * mdurcan
 * 
 */

// micro-stepping
int microStepping           = 1;    // 1: full-step
                                    // 2: 1/2-step
                                    // 3: 1/4-step
                                    // 4: 1/8-step
                                    // 5: 1/16-step

// delay
int stepperDelay            = 300;  // microseconds

// initial direction
bool directionUP            = true; // true:UP false:DOWN

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
int pin_A_MS1               = x;
int pin_A_MS2               = x;
int pin_A_MS3               = x;

// stepper B                
int pin_B_ENABLE            = 2;
int pin_B_DIR               = 4;
int pin_B_STEP              = 3;
int pin_B_MS1               = x;
int pin_B_MS2               = x;
int pin_B_MS3               = x;

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
	
	// set micro-stepping
	setMicrostepping(microStepping);

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

        delay(300);

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
        if( readUpDownButton() ) { setStepperDelay(500); return; }
        
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

void setStepperDelay(int d)
{
    stepperDelay = d;
}

bool isStopperHit_A_up()
{
    if ( !digitalRead(pin_stopper_A_up) )
    {
        Serial.println("stopper A-up hit...");
        setStepperDelay(300);
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
        setStepperDelay(300);
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
        setStepperDelay(300);
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
        setStepperDelay(300);
        return true;
    }
    else
    {
        return false;
    }
}

void setMicrostepping(int s){
  switch (s){
    case 1:
      Serial.print("full step...");
	  
      digitalWrite(pin_A_MS1, LOW);
      digitalWrite(pin_A_MS2, LOW);
      digitalWrite(pin_A_MS3, LOW);
	  
      digitalWrite(pin_B_MS1, LOW);
      digitalWrite(pin_B_MS2, LOW);
      digitalWrite(pin_B_MS3, LOW);
	  
      break;
    case 2:
      Serial.print("half step...");
	  
      digitalWrite(pin_A_MS1, HIGH);
      digitalWrite(pin_A_MS2, LOW);
      digitalWrite(pin_A_MS3, LOW);
	  
      digitalWrite(pin_B_MS1, HIGH);
      digitalWrite(pin_B_MS2, LOW);
      digitalWrite(pin_B_MS3, LOW);
	  
      break;
    case 3:
      Serial.print("quarter step...");
	  
      digitalWrite(pin_A_MS1, LOW);
      digitalWrite(pin_A_MS2, HIGH);
      digitalWrite(pin_A_MS3, LOW);
	  
      digitalWrite(pin_B_MS1, LOW);
      digitalWrite(pin_B_MS2, HIGH);
      digitalWrite(pin_B_MS3, LOW);
	  
      break;
    case 4:
      Serial.print("8th step...");
	  
      digitalWrite(pin_A_MS1, HIGH);
      digitalWrite(pin_A_MS2, HIGH);
      digitalWrite(pin_A_MS3, LOW);
	  
      digitalWrite(pin_B_MS1, HIGH);
      digitalWrite(pin_B_MS2, HIGH);
      digitalWrite(pin_B_MS3, LOW);
	  
      break;
    case 5:
      Serial.print("16th step...");
	  
      digitalWrite(pin_A_MS1, HIGH);
      digitalWrite(pin_A_MS2, HIGH);
      digitalWrite(pin_A_MS3, HIGH);
	  
      digitalWrite(pin_B_MS1, HIGH);
      digitalWrite(pin_B_MS2, HIGH);
      digitalWrite(pin_B_MS3, HIGH);
	  
      break;
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