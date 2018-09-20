/*
 * ver 2.0 20-09-2018
 * mdurcan
 * 
 */

int pin_led            = 13;
int pin_buttonUp       = 14;
int pin_buttonDown     = 15;

//stepper:lift
int pin_Kenable        = 2;
int pin_Kdir           = 3;
int pin_Kstep          = 4;

int pin_Lenable        = 5;
int pin_Ldir           = 6;
int pin_Lstep          = 7;

int pin_Menable        = 8;
int pin_Mdir           = 9;
int pin_Mstep          = 10;

int stepLift           = 1000;
int delayStepLift      = 10; //milliseconds


void setup() {
  Serial.begin(9600);
  
  // button
  pinMode(pin_buttonUp, INPUT);
  pinMode(pin_buttonDown, INPUT);
  
  // led
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);

  //stepper:lift
  pinMode(pin_Kenable, OUTPUT);
  pinMode(pin_Lenable, OUTPUT);
  pinMode(pin_Menable, OUTPUT);
  
  pinMode(pin_Kdir, OUTPUT);
  pinMode(pin_Ldir, OUTPUT);
  pinMode(pin_Mdir, OUTPUT);
  
  pinMode(pin_Kstep, OUTPUT);
  pinMode(pin_Lstep, OUTPUT);
  pinMode(pin_Lstep, OUTPUT);
  
  // LOW:enable, HIGH:disable
  digitalWrite(pin_Kenable, HIGH);
  digitalWrite(pin_Lenable, HIGH);
  digitalWrite(pin_Menable, HIGH);
  
  // LOW:clockwise, HIGH:anti-clockwise
  digitalWrite(pin_Kdir, LOW);
  digitalWrite(pin_Ldir, LOW);
  digitalWrite(pin_Mdir, LOW);
  
  digitalWrite(pin_Kstep, LOW);
  digitalWrite(pin_Lstep, LOW);
  digitalWrite(pin_Mstep, LOW);
}

void loop() {

  if (readButton(pin_buttonUp)==1) { // up
    Serial.println("lift > up");
	controlLed(true);
    driveLift(true); //up
    delay(1000);
    Serial.println("ready > opened");
	controlLed(false);
  }
  if (readButton(pin_buttonDown)==1) { // down
    Serial.println("lift > down");
	controlLed(true);
    driveLift(false); //down
    delay(1000);
    Serial.println("ready > closed");
	controlLed(false);
  }
  
}

int readButton(pin) {
  return digitalRead(pin);
}

void controlLed(bool onOff) { // on:true, off:false
  digitalWrite(pin_led, onOff ? HIGH : LOW);
}


  digitalWrite(pin_AIN1, LOW);
  digitalWrite(pin_AIN2, LOW);
  digitalWrite(pin_AIN3, LOW);
  digitalWrite(pin_AIN4, LOW);

  digitalWrite(pin_BIN1, LOW);
  digitalWrite(pin_BIN2, LOW);
  digitalWrite(pin_BIN3, LOW);
  digitalWrite(pin_BIN4, LOW);
}

void driveLift(bool clockwise) {
  // enable driver (LOW:enable, HIGH:disable)
  digitalWrite(pin_Kenable, LOW);
  digitalWrite(pin_Lenable, LOW);
  digitalWrite(pin_Menable, LOW);

  // direction (LOW:clockwise, HIGH:anti-clockwise)
  digitalWrite(pin_Kdir, clockwise?LOW:HIGH);
  digitalWrite(pin_Ldir, clockwise?LOW:HIGH);
  digitalWrite(pin_Mdir, clockwise?LOW:HIGH);
  
  for (int j = 0; j < stepLift; j++) {
    digitalWrite(pin_Kstep, HIGH);
    digitalWrite(pin_Lstep, HIGH);
    digitalWrite(pin_Mstep, HIGH);
    delay(delayStepLift);
    
    digitalWrite(pin_Kstep, LOW);
    digitalWrite(pin_Lstep, LOW);
    digitalWrite(pin_Mstep, LOW);
    delay(delayStepLift);
  }

  stopLift();
}

void stopLift() {
  digitalWrite(pin_Kenable, HIGH);
  digitalWrite(pin_Lenable, HIGH);
  digitalWrite(pin_Menable, HIGH);

  digitalWrite(pin_Kstep, LOW);
  digitalWrite(pin_Lstep, LOW);
  digitalWrite(pin_Mstep, LOW);

  digitalWrite(pin_Kdir, LOW);
  digitalWrite(pin_Ldir, LOW);
  digitalWrite(pin_Mdir, LOW);
}
