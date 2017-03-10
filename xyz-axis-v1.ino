/*
 * 10-03-2017
 * YK42BYGH28-0404A13-X
 * 3-axis control
*/

// pot
int pin_pot  = A0;
int potValue = 0; // 1K: 0-1023

// joystick XY
int pin_xJoy        = A1;
int pin_yJoy        = A2;
int pin_joyXYbutton = 22;
int xPos            = 0;
int yPos            = 0;
int buttonXYvalue   = 0;

// stepper XYZ
int pin_Xstep  = 30;
int pin_Xdir   = 31;
int pin_Ystep  = 32;
int pin_Ydir   = 33;
int pin_Zstep  = 34;
int pin_Zdir   = 35;

int stepDelay=10; // microseconds

void setup() {
  Serial.begin(9600);
  
  // pot
  pinMode(pin_pot, INPUT);
  
  // joystick XY
  pinMode(pin_xJoy, INPUT);
  pinMode(pin_yJoy, INPUT);
  pinMode(pin_joyXYbutton, INPUT_PULLUP); //activate pull-up resistor on the push-button pin
  
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
}

void loop() {
  readPot();
  readJoystickXY();
  
  controlStepper();

  //monitor();
}

void monitor(){
  Serial.print("Pot: "); Serial.print(potValue);
  Serial.print(" | X: "); Serial.print(xPos);
  Serial.print(" | Y: "); Serial.print(yPos);
  Serial.print(" | ButtonXY: "); Serial.print(buttonXYvalue);
  //Serial.print(" | stepX: ");
  //Serial.println(stepperXstep);
  Serial.println(" _");
}
void readPot(){
  potValue = analogRead(pin_pot);
}
void readJoystickXY(){
  xPos = analogRead(pin_xJoy);
  yPos = analogRead(pin_yJoy);
  buttonXYvalue = digitalRead(pin_joyXYbutton);
}


void controlStepper(){

       if (xPos<490) { driveXYZ('X',true); }
  else if (xPos>535) { driveXYZ('X',false); }
  else if (yPos<490) { driveXYZ('Y',true); }
  else if (yPos>535) { driveXYZ('Y',false); }
  else if (buttonXYvalue==0) {driveXYZ('Z',true); }
  else               { stopXYZ(); }

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
  digitalWrite(_pin_dir, clockwise?LOW:HIGH);
  
  digitalWrite(_pin_step, HIGH);
    delayMicroseconds(stepDelay);
  digitalWrite(_pin_step, LOW);
    delayMicroseconds(stepDelay);
}

void stopXYZ(){
  digitalWrite(pin_Xstep, LOW);
  digitalWrite(pin_Ystep, LOW);
  digitalWrite(pin_Zstep, LOW);
}

/*
void driveX(bool clockwise){
  digitalWrite(pin_Xdir, clockwise?LOW:HIGH);
  
  digitalWrite(pin_Xstep, HIGH);
    delayMicroseconds(stepDelay);
  digitalWrite(pin_Xstep, LOW);
    delayMicroseconds(stepDelay);
}
*/
