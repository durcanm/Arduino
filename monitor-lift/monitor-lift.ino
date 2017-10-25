/*
  ver 1.0 21-10-2017
*/

int liftState          = 0; // 0:closed, 1:opened, -1:busy

int pin_button         = 11;
int pin_led            = 13;

//stepper:cover
int pin_AIN1           = 18;
int pin_AIN2           = 19;
int pin_AIN3           = 20;
int pin_AIN4           = 21;

int pin_BIN1           = 14;
int pin_BIN2           = 15;
int pin_BIN3           = 16;
int pin_BIN4           = 17;

int stepCover          = 900;
int currentStepCover   = 0;
int delayStepCover     = 8; //milliseconds

//stepper:lift
int pin_Kenable        = 8;
int pin_Kstep          = 9;
int pin_Kdir           = 10;

int pin_Lenable        = 5;
int pin_Lstep          = 6;
int pin_Ldir           = 7;

int pin_Menable        = 2;
int pin_Mstep          = 3;
int pin_Mdir           = 4;

int stepLift           = 500;
int delayStepLift      = 10; //milliseconds


void setup() {
  Serial.begin(9600);
  
  // button
  pinMode(pin_button, INPUT);
  
  // led
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);

  //stepper:cover
  pinMode(pin_AIN1, OUTPUT);
  pinMode(pin_AIN2, OUTPUT);
  pinMode(pin_AIN3, OUTPUT);
  pinMode(pin_AIN4, OUTPUT);

  digitalWrite(pin_AIN1, LOW);
  digitalWrite(pin_AIN2, LOW);
  digitalWrite(pin_AIN3, LOW);
  digitalWrite(pin_AIN4, LOW);

  pinMode(pin_BIN1, OUTPUT);
  pinMode(pin_BIN2, OUTPUT);
  pinMode(pin_BIN3, OUTPUT);
  pinMode(pin_BIN4, OUTPUT);

  digitalWrite(pin_BIN1, LOW);
  digitalWrite(pin_BIN2, LOW);
  digitalWrite(pin_BIN3, LOW);
  digitalWrite(pin_BIN4, LOW);
  
  //stepper:lift
  pinMode(pin_Kenable, OUTPUT);
  pinMode(pin_Lenable, OUTPUT);
  pinMode(pin_Menable, OUTPUT);
  
  pinMode(pin_Kstep, OUTPUT);
  pinMode(pin_Lstep, OUTPUT);
  pinMode(pin_Lstep, OUTPUT);
  
  pinMode(pin_Kdir, OUTPUT); // LOW:clockwise, HIGH:anti-clockwise
  pinMode(pin_Ldir, OUTPUT);
  pinMode(pin_Mdir, OUTPUT);
  
  digitalWrite(pin_Kenable, HIGH); // LOW:enable, HIGH:disable
  digitalWrite(pin_Lenable, HIGH);
  digitalWrite(pin_Menable, HIGH);
  digitalWrite(pin_Kstep, LOW);
  digitalWrite(pin_Lstep, LOW);
  digitalWrite(pin_Mstep, LOW);
  digitalWrite(pin_Kdir, LOW);
  digitalWrite(pin_Ldir, LOW);
  digitalWrite(pin_Mdir, LOW);
}

void loop() {

  if (readButton()==1 && liftState==0){
    //open
    liftState = -1; // busy

    Serial.println("led > on");
    controlLed(true);
    delay(1000);

    Serial.println("cover > open");
    driveCover(true); //open
    delay(1000);

    Serial.println("lift > up");
    driveLift(true); //up
    delay(1000);

    liftState = 1; // opened
    Serial.println("ready > opened");
  }
  if (readButton()==1 && liftState==1){
    //close
    liftState = -1; // busy

    Serial.println("lift > down");
    driveLift(false); //down
    delay(1000);

    Serial.println("cover > close");
    driveCover(false); //close
    delay(1000);

    Serial.println("led > off");
    controlLed(false);
    delay(1000);
    
    liftState = 0; // closed
    Serial.println("ready > closed");
  }
  
}

int readButton() {
  return digitalRead(pin_button);
}

void controlLed(bool onOff) { // on:true, off:false
  digitalWrite(pin_led, onOff ? HIGH : LOW);
}

void driveCover(bool clockwise) {
  
  for (int i=0; i<stepCover; i++) {
  int directionStep = clockwise ? currentStepCover : (4-1)-currentStepCover;

  //Serial.print("directionStep: "); Serial.print(directionStep);

  switch(directionStep){
    case 0:
    digitalWrite(pin_AIN1, HIGH);
    digitalWrite(pin_AIN2, LOW);
    digitalWrite(pin_AIN3, LOW);
    digitalWrite(pin_AIN4, LOW);

    digitalWrite(pin_BIN1, LOW);
    digitalWrite(pin_BIN2, LOW);
    digitalWrite(pin_BIN3, LOW);
    digitalWrite(pin_BIN4, HIGH);
    break;
    case 1:
    digitalWrite(pin_AIN1, LOW);
    digitalWrite(pin_AIN2, HIGH);
    digitalWrite(pin_AIN3, LOW);
    digitalWrite(pin_AIN4, LOW);

    digitalWrite(pin_BIN1, LOW);
    digitalWrite(pin_BIN2, LOW);
    digitalWrite(pin_BIN3, HIGH);
    digitalWrite(pin_BIN4, LOW);
    break;
    case 2:
    digitalWrite(pin_AIN1, LOW);
    digitalWrite(pin_AIN2, LOW);
    digitalWrite(pin_AIN3, HIGH);
    digitalWrite(pin_AIN4, LOW);

    digitalWrite(pin_BIN1, LOW);
    digitalWrite(pin_BIN2, HIGH);
    digitalWrite(pin_BIN3, LOW);
    digitalWrite(pin_BIN4, LOW);
    break;
    case 3:
    digitalWrite(pin_AIN1, LOW);
    digitalWrite(pin_AIN2, LOW);
    digitalWrite(pin_AIN3, LOW);
    digitalWrite(pin_AIN4, HIGH);

    digitalWrite(pin_BIN1, HIGH);
    digitalWrite(pin_BIN2, LOW);
    digitalWrite(pin_BIN3, LOW);
    digitalWrite(pin_BIN4, LOW);
    break;
  }

  currentStepCover = (++currentStepCover < 4) ? currentStepCover : 0;

  delay(delayStepCover);
  
  }
  
  stopCover();
}

void stopCover() {
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
  digitalWrite(pin_Kenable, LOW); delayMicroseconds(10);
  digitalWrite(pin_Lenable, LOW); delayMicroseconds(10);
  digitalWrite(pin_Menable, LOW); delayMicroseconds(10);

  // Y:direction (LOW:clockwise, HIGH:anti-clockwise)
  digitalWrite(pin_Kdir, clockwise?LOW:HIGH);
  digitalWrite(pin_Ldir, clockwise?LOW:HIGH);
  digitalWrite(pin_Mdir, clockwise?LOW:HIGH);
  
  for (int j = 0; j < stepLift; j++) {
    digitalWrite(pin_Kstep, HIGH); delayMicroseconds(10);
    digitalWrite(pin_Lstep, HIGH); delayMicroseconds(10);
    //digitalWrite(pin_Mstep, HIGH); delayMicroseconds(5);
    delay(delayStepLift);
    
    digitalWrite(pin_Kstep, LOW); delayMicroseconds(10);
    digitalWrite(pin_Lstep, LOW); delayMicroseconds(10);
    //digitalWrite(pin_Mstep, LOW); delayMicroseconds(5);
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
