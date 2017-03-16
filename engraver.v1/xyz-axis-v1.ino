/*
 * 10-03-2017
 * YK42BYGH28-0404A13-X
 * 3-axis control
*/

// stepper led
int pin_ledStepper   = 2;

// pot
int pin_pot          = A0;
int potValue         = 0; // 1K: 0-1023

// joystick XY
int pin_xJoy         = A1;
int pin_yJoy         = A2;
int pin_joyXYbutton  = 52;
int xPos             = 0;
int yPos             = 0;
int buttonXYvalue    = 0;

// joystick Z
int pin_zJoy         = A4;
int pin_wJoy         = A3;
int pin_joyZbutton   = 53;
int zPos             = 0;
int buttonZvalue     = 0;

// stepper XYZ
int pin_Xstep        = 30;
int pin_Xdir         = 31;
int pin_Ystep        = 32;
int pin_Ydir         = 33;
int pin_Zstep        = 34;
int pin_Zdir         = 35;
int stepDelay        = 10; // microseconds

// dc button
int pin_dcButton     = 49;
int buttonDCvalue    = 0;

// dc motor
int pin_dcA          = 40;
int pin_dcB          = 41;

// auto-drive
int pin_ledAuto      = 3;
int pin_auto         = pin_joyZbutton; //48;
int buttonAutoValue  = 0;
bool runAutoDrive    = true;

void setup() {
  Serial.begin(9600);
  
  // led
  pinMode(pin_ledStepper, OUTPUT);
  digitalWrite(pin_ledStepper, LOW);
  
  pinMode(pin_ledAuto, OUTPUT);
  digitalWrite(pin_ledAuto, LOW);
  
  // pot
  pinMode(pin_pot, INPUT);
  
  // joystick XY
  pinMode(pin_xJoy, INPUT);
  pinMode(pin_yJoy, INPUT);
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
    Serial.print(" | ButtonXY: "); Serial.print(buttonXYvalue);
    Serial.print(" | ButtonDC: "); Serial.print(buttonDCvalue);
    Serial.println(" _");
}

void readPot(){
  potValue = analogRead(pin_pot);
}
void readJoystick(){
  xPos = analogRead(pin_xJoy);
  yPos = analogRead(pin_yJoy);
  buttonXYvalue = digitalRead(pin_joyXYbutton);

  zPos = analogRead(pin_zJoy);
  buttonZvalue = digitalRead(pin_joyZbutton);
}
void readButtonDC(){
  buttonDCvalue = digitalRead(pin_dcButton);
}
void readButtonAuto(){
  buttonAutoValue = !digitalRead(pin_auto);
}
void controlStepperLed(bool on){ // on:true, off:false
  digitalWrite(pin_ledStepper, on?HIGH:LOW);
}
void controlAutoLed(bool on){ // on:true, off:false
  digitalWrite(pin_ledAuto, on?HIGH:LOW);
}

void controlXYZ(){

  stepDelay = (int)potValue; // calculate stepper delay here

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
  if (buttonDCvalue == HIGH) { driveDC(); }
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
  //if (!runAutoDrive) { return; }
  if (buttonAutoValue == LOW) { return; }
  
  runAutoDrive = false;
  int _xMove=3200; // 3200:1 rotation
  int _yMove=1600;
  int _yLoop=4;
  int _zMove=1000;
  int _wait=300;  // milliseconds
  int _delay=100; // microseconds

  
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
