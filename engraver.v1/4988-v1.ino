/*
 * 10-03-2017
 * rev: 17-03-2017
 * YK42BYGH28-0404A13-X
 * 3-axis control
 */


// joystick XY
int pin_joyX         = A1;
int pin_joyY         = A2;
int pin_joyXYbutton  = 51;
int xPos             = 0; // 0-1023
int yPos             = 0; // 0-1023
int XYbuttonValue    = 0;

// joystick Z
int pin_joyZ         = A3;
int pin_joyZbutton   = 53;
int zPos             = 0; // 0-1023
int ZbuttonValue     = 0;

// stepper XYZ
int pin_Xstep        = 22;
int pin_Xdir         = 24;
int pin_Ystep        = 26;
int pin_Ydir         = 28;
int pin_Zstep        = 30;
int pin_Zdir         = 32;
int stepDelay        = 10; // microseconds, configured via pot

// dc motor
int pin_dcA          = 38;
int pin_dcB          = 40;

// pot
int pin_pot          = A0;
int potValue         = 0; // 1K: 0-1023

// stepper led
int pin_stepperLed   = 35;

// dc button (green)
int pin_dcButton     = 47;
int DCbuttonValue    = 0;

// auto-drive (blue)
int pin_ledAuto      = 37;
int pin_auto         = 49;
int autoButtonValue  = 0;
bool runAutoDrive    = true;


void setup() {
  Serial.begin(9600);
  
  // led
  pinMode(pin_stepperLed, OUTPUT);
  digitalWrite(pin_stepperLed, LOW);
  
  pinMode(pin_ledAuto, OUTPUT);
  digitalWrite(pin_ledAuto, LOW);
  
  // pot
  pinMode(pin_pot, INPUT);
  
  // joystick XY
  pinMode(pin_joyX, INPUT);
  pinMode(pin_joyY, INPUT);
  pinMode(pin_joyXYbutton, INPUT_PULLUP); //activate pull-up resistor on the push-button pin
  pinMode(pin_joyZbutton, INPUT_PULLUP);
  
  // stepper XYZ
  pinMode(pin_Xstep, OUTPUT);
  pinMode(pin_Xdir, OUTPUT);
  pinMode(pin_Ystep, OUTPUT);
  pinMode(pin_Ydir, OUTPUT);
  pinMode(pin_Zstep, OUTPUT);
  pinMode(pin_Zdir, OUTPUT);
  
  digitalWrite(pin_Xstep, LOW);
  digitalWrite(pin_Xdir, LOW);
  digitalWrite(pin_Ystep, LOW);
  digitalWrite(pin_Ydir, LOW);
  digitalWrite(pin_Zstep, LOW);
  digitalWrite(pin_Zdir, LOW);

  // dc button
  pinMode(pin_dcA, pin_dcButton);
  
  // dc motor
  pinMode(pin_dcA, OUTPUT);
  pinMode(pin_dcB, OUTPUT);

  digitalWrite(pin_dcA, LOW);
  digitalWrite(pin_dcB, LOW);
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
    Serial.print(" | ButtonXY: "); Serial.print(XYbuttonValue);
    Serial.print(" | ButtonDC: "); Serial.print(DCbuttonValue);
    Serial.println(" _");
}

void readPot(){
  potValue = analogRead(pin_pot);
}
void readJoystick(){
  xPos = analogRead(pin_joyX);
  yPos = analogRead(pin_joyY);
  XYbuttonValue = digitalRead(pin_joyXYbutton);

  zPos = analogRead(pin_joyZ);
  ZbuttonValue = digitalRead(pin_joyZbutton);
}
void readButtonDC(){
  DCbuttonValue = digitalRead(pin_dcButton);
}
void readButtonAuto(){
  autoButtonValue = digitalRead(pin_auto);
}
void controlStepperLed(bool on){ // on:true, off:false
  digitalWrite(pin_stepperLed, on?HIGH:LOW);
}
void controlAutoLed(bool on){ // on:true, off:false
  digitalWrite(pin_ledAuto, on?HIGH:LOW);
}

void controlXYZ(){

  stepDelay = (int)potValue*15+1000; // calculate stepper delay here

       if (xPos<490)  { driveXYZ('X',true); }
  else if (xPos>535)  { driveXYZ('X',false); }
  else if (yPos<490)  { driveXYZ('Y',true); }
  else if (yPos>535)  { driveXYZ('Y',false); }
  else if (zPos>535)  { stepDelay=5000; driveXYZ('Z',true); } // Z axis down-speed is fixed and slower
  else if (zPos<490)  { stepDelay=1000; driveXYZ('Z',false); } // Z axis up-speed is fixed and faster
  else                { stopXYZ(); }

}
void driveXYZ(char axis, bool clockwise){
  int _pin_dir=-1;
  int _pin_step=-1;
  
  switch(axis){
    case 'X':
      _pin_dir=pin_Xdir;
      _pin_step=pin_Xstep;
      break;
    case 'Y':
      _pin_dir=pin_Ydir;
      _pin_step=pin_Ystep;
      break;
    case 'Z':
      _pin_dir=pin_Zdir;
      _pin_step=pin_Zstep;
      break;
  }
  controlStepperLed(true);
  
  digitalWrite(_pin_dir, clockwise?LOW:HIGH); // motor direction control
  
  digitalWrite(_pin_step, HIGH);
    delayMicroseconds(stepDelay);
  digitalWrite(_pin_step, LOW);
    delayMicroseconds(stepDelay);
}
void stopXYZ(){
  controlStepperLed(false);
  
  digitalWrite(pin_Xstep, LOW);
  digitalWrite(pin_Ystep, LOW);
  digitalWrite(pin_Zstep, LOW);
  
  digitalWrite(pin_Xdir, LOW);
  digitalWrite(pin_Ydir, LOW);
  digitalWrite(pin_Zdir, LOW);
}


void controlDC(){  
  if (DCbuttonValue == HIGH) { driveDC(); }
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
  int _xMove=800; // 400:1 rotation
  int _yMove=400;
  int _zMove=200;
  int _yLoop=4;
  int _wait=300;  // milliseconds
  int _delay=6300; // microseconds

  
  Serial.println("auto-start");
  controlAutoLed(true);

  
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

  
  Serial.println("auto-stop");
  controlAutoLed(false);
}
