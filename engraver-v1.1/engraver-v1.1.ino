/*
 * 09-04-2017 revision: 10-04-2017
 * 17HS3001-20B stepper
 * A4988 driver
 * 3-axis joystick control
 * https://github.com/durcanm/Arduino
 */


// pot
int pin_pot          = A0;
int potValue         = 0; // 1K: 0-1023

// joystick XYZ
int pin_joyX         = A1;
int pin_joyY         = A2;
int pin_joyZ         = A3;
int xPos             = 0; // 0-1023
int yPos             = 0; // 0-1023
int zPos             = 0; // 0-1023

// stepper XYZ
int pin_Xenable      = 22;
int pin_Xms1         = 24;
int pin_Xstep        = 26;
int pin_Xdir         = 28;
int pin_Yenable      = 23;
int pin_Yms1         = 25;
int pin_Ystep        = 27;
int pin_Ydir         = 29;
int pin_Zenable      = 30;
int pin_Zms1         = 32;
int pin_Zstep        = 34;
int pin_Zdir         = 36;
int stepDelay        = 10; // microseconds, configured via pot

// dc motor
int pin_dcA          = 38;
int pin_dcB          = 40;

// led:stepper
int pin_stepperLed   = 43;

// led:auto-drive
int pin_autoLed      = 45;

// button:auto-drive (blue)
int pin_autoButton   = 47;
int autoButtonValue  = 0;
bool runAutoDrive    = true;

// button:dc button (green)
int pin_dcButton     = 49;
int dcButtonValue    = 0;


void setup() {  
  // pot
  pinMode(pin_pot, INPUT);
  
  // joystick XYZ
  pinMode(pin_joyX, INPUT);
  pinMode(pin_joyY, INPUT);
  pinMode(pin_joyZ, INPUT);
  
  // led
  pinMode(pin_stepperLed, OUTPUT);
  digitalWrite(pin_stepperLed, LOW);
  
  pinMode(pin_autoLed, OUTPUT);
  digitalWrite(pin_autoLed, LOW);
  
  // stepper XYZ
  pinMode(pin_Xenable, OUTPUT);
  pinMode(pin_Yenable, OUTPUT);
  pinMode(pin_Zenable, OUTPUT);
  
  pinMode(pin_Xms1, OUTPUT);
  pinMode(pin_Yms1, OUTPUT);
  pinMode(pin_Zms1, OUTPUT);
  
  pinMode(pin_Xstep, OUTPUT);
  pinMode(pin_Ystep, OUTPUT);
  pinMode(pin_Zstep, OUTPUT);
  
  pinMode(pin_Ydir, OUTPUT);
  pinMode(pin_Xdir, OUTPUT);
  pinMode(pin_Zdir, OUTPUT);  

  digitalWrite(pin_Xenable, HIGH); // HIGH:disable, LOW:enable
  digitalWrite(pin_Yenable, HIGH);
  digitalWrite(pin_Zenable, HIGH);

  digitalWrite(pin_Xms1, LOW);
  digitalWrite(pin_Yms1, LOW);
  digitalWrite(pin_Zms1, LOW);
  
  digitalWrite(pin_Xstep, LOW);
  digitalWrite(pin_Ystep, LOW);
  digitalWrite(pin_Zstep, LOW);
  
  digitalWrite(pin_Xdir, LOW); // LOW:clockwise, HIGH:anti-clockwise  
  digitalWrite(pin_Ydir, LOW);  
  digitalWrite(pin_Zdir, LOW);

  // auto-drive button
  pinMode(pin_autoButton, INPUT);

  // dc button
  pinMode(pin_dcButton, INPUT);
  
  // dc motor
  pinMode(pin_dcA, OUTPUT);
  pinMode(pin_dcB, OUTPUT);
  
  digitalWrite(pin_dcA, LOW);
  digitalWrite(pin_dcB, LOW);
  
  // monitor
  Serial.begin(9600);
}

void loop() {
  readPot();
  readJoystick();
  readButtonDC();
  readButtonAuto();
  
  controlXYZ();
  controlDC();

  autoDrive();

  //monitor();
}



void monitor(){
    Serial.print("Pot: "); Serial.print(potValue);
    Serial.print(" | X: "); Serial.print(xPos);
    Serial.print(" | Y: "); Serial.print(yPos);
    Serial.print(" | ButtonDC: "); Serial.print(dcButtonValue);
    Serial.println(" _");
}

void readPot(){
  potValue = analogRead(pin_pot);
}
void readJoystick(){
  xPos = analogRead(pin_joyX);
  yPos = analogRead(pin_joyY);
  zPos = analogRead(pin_joyZ);
}
void readButtonDC(){
  dcButtonValue = digitalRead(pin_dcButton);
}
void readButtonAuto(){
  autoButtonValue = digitalRead(pin_autoButton);
}
void controlStepperLed(bool on){ // on:true, off:false
  digitalWrite(pin_stepperLed, on?HIGH:LOW);
}
void controlAutoLed(bool on){ // on:true, off:false
  digitalWrite(pin_autoLed, on?HIGH:LOW);
}

void controlXYZ(){

  stepDelay = (int)potValue/35+1; // calculate stepper delay here, values:1-30
  //Serial.println(stepDelay);

       if (xPos<490)  { driveXYZ('X',true); }
  else if (xPos>535)  { driveXYZ('X',false); }
  else if (yPos<490)  { driveXYZ('Y',true); }
  else if (yPos>535)  { driveXYZ('Y',false); }
  else if (zPos>535)  { stepDelay=50; driveXYZ('Z',true); } // Z axis down-speed is fixed and slower
  else if (zPos<490)  { stepDelay=10; driveXYZ('Z',false); } // Z axis up-speed is fixed and faster
  else                { stopXYZ(); }

}
void driveXYZ(char axis, bool clockwise){
  int _pin_enable=-1;
  int _pin_dir=-1;
  int _pin_step=-1;
  
  switch(axis){
    case 'X':
      _pin_enable=pin_Xenable;
	  _pin_dir=pin_Xdir;
      _pin_step=pin_Xstep;
      break;
    case 'Y':
	  _pin_enable=pin_Yenable;
      _pin_dir=pin_Ydir;
      _pin_step=pin_Ystep;
      break;
    case 'Z':
	  _pin_enable=pin_Zenable;
      _pin_dir=pin_Zdir;
      _pin_step=pin_Zstep;
      break;
  }
  controlStepperLed(true);
  
  digitalWrite(_pin_enable, LOW); // enable driver
  delayMicroseconds(5);
  
  digitalWrite(_pin_dir, clockwise?LOW:HIGH); // motor direction control
  delayMicroseconds(5);
  
  digitalWrite(_pin_step, HIGH);
    delay(stepDelay); //delayMicroseconds(stepDelay);
  digitalWrite(_pin_step, LOW);
    delay(stepDelay); //delayMicroseconds(stepDelay);
}
void stopXYZ(){
  controlStepperLed(false);
  
  digitalWrite(pin_Xenable, HIGH);
  digitalWrite(pin_Yenable, HIGH);
  digitalWrite(pin_Zenable, HIGH);
  
  digitalWrite(pin_Xstep, LOW);
  digitalWrite(pin_Ystep, LOW);
  digitalWrite(pin_Zstep, LOW);
  
  digitalWrite(pin_Xdir, LOW);
  digitalWrite(pin_Ydir, LOW);
  digitalWrite(pin_Zdir, LOW);
}


void controlDC(){  
  if (dcButtonValue == HIGH) { driveDC(); }
  else                       { stopDC(); }
}
void driveDC(){
  digitalWrite(pin_dcA, HIGH);
  digitalWrite(pin_dcB, LOW);
}
void stopDC(){
  digitalWrite(pin_dcA, LOW);
  digitalWrite(pin_dcB, LOW);
}

void autoDrive(){
  //if (!runAutoDrive) { return; } // to run one-time only
  if (autoButtonValue == LOW) { return; }
  
  runAutoDrive = false;
  int _xMove = 400; // 400:1 rotation fullstep
  int _yMove = 200;
  int _zMove = 100;
  int _yLoop = 4;
  int _wait  = 300;  // milliseconds
  int _delay = 700; // microseconds

  
  Serial.println("auto-start");
  controlAutoLed(true);

  //enable driver
  digitalWrite(pin_Xenable,LOW);
  digitalWrite(pin_Yenable,LOW);
  digitalWrite(pin_Zenable,LOW);
  
  digitalWrite(pin_Zdir, LOW);  // Z-clockwise;
  for(int i=0;i<_zMove;i++){
    digitalWrite(pin_Zstep, HIGH); delayMicroseconds(_delay);
    digitalWrite(pin_Zstep, LOW); delayMicroseconds(_delay);
  }
  delay(_wait);
  for(int j=0;j<_yLoop;j++){
    digitalWrite(pin_Xdir, LOW);  // X-clockwise;
    for(int i=0;i<_xMove;i++){
      digitalWrite(pin_Xstep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Xstep, LOW); delayMicroseconds(_delay);
    }
    delay(_wait);
    for(int i=0;i<_yMove;i++){
      digitalWrite(pin_Ystep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Ystep, LOW); delayMicroseconds(_delay);
    }
    delay(_wait);
    digitalWrite(pin_Xdir, HIGH);  // X-anticlockwise;
    for(int i=0;i<_xMove;i++){
      digitalWrite(pin_Xstep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Xstep, LOW); delayMicroseconds(_delay);
    }
    delay(_wait);
    for(int i=0;i<_yMove;i++){
      digitalWrite(pin_Ystep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Ystep, LOW); delayMicroseconds(_delay);
    }
    delay(_wait);
  }    
  digitalWrite(pin_Zdir, HIGH);  // Z-anticlockwise;
  for(int i=0;i<_zMove;i++){
    digitalWrite(pin_Zstep, HIGH); delayMicroseconds(_delay);
    digitalWrite(pin_Zstep, LOW); delayMicroseconds(_delay);
  }

  stopXYZ();
  
  Serial.println("auto-stop");
  controlAutoLed(false);
}

