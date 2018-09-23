// stepper K
int pin_K_ENABLE = 2;
int pin_K_DIR    = 4;
int pin_K_STEP   = 3;

// stepper L
int pin_L_ENABLE = 5;
int pin_L_DIR    = 7;
int pin_L_STEP   = 6;

// led
int pin_led            = 50;

// button
int pin_buttonUp       = 52;
//int pin_buttonDown     = 15;

// settings
int stepCount  = 400; // 400:1 rotation in full-step mode
bool clockwise = true;


void setup() {
    Serial.begin(9600);

    // button
    pinMode(pin_buttonUp, INPUT);

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

void loop() {  

    if (digitalRead(pin_buttonUp))
    {
        Serial.println("stop...");

        digitalWrite(pin_K_ENABLE, LOW);
        digitalWrite(pin_L_ENABLE, LOW);
        
        delayMicroseconds(2);

        digitalWrite(pin_K_DIR, clockwise?LOW:HIGH);
        digitalWrite(pin_L_DIR, clockwise?LOW:HIGH);

        if(clockwise){clockwise=false;} else {clockwise=true;} 


        for(int i=0; i<stepCount; i++)
        {        
            // led ON
            digitalWrite(pin_led, HIGH);

            //Serial.print(i);    
            digitalWrite(pin_K_STEP, HIGH);
            delayMicroseconds(5);
            digitalWrite(pin_L_STEP, HIGH);
            delayMicroseconds(700);

            digitalWrite(pin_K_STEP, LOW);
            delayMicroseconds(5);
            digitalWrite(pin_L_STEP, LOW);
            delayMicroseconds(700);
        }

        digitalWrite(pin_K_ENABLE, HIGH);
        digitalWrite(pin_K_DIR, LOW);
        digitalWrite(pin_K_STEP, LOW);

        digitalWrite(pin_L_ENABLE, HIGH);
        digitalWrite(pin_L_DIR, LOW);
        digitalWrite(pin_L_STEP, LOW);

        delayMicroseconds(2);

        // led OFF
        digitalWrite(pin_led, LOW);

        Serial.println("stop...");
        delay(500);
    }
}
