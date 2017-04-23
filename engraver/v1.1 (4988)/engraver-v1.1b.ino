/*
  09-04-2017 revision: 15-04-2017
  17HS3001-20B stepper
  A4988 (stepper) L298 (dc)
  3-axis joystick control
  https://github.com/durcanm/Arduino
*/


// pot
int pin_pot          = A0;
int potValue         = 0; // 1K: 0-1023

// joystick XYZ
int pin_joyX         = A1;
int pin_joyY         = A2;
int pin_joyZ         = A3;
int posX             = 0; // 0-1023
int posY             = 0; // 0-1023
int posZ             = 0; // 0-1023

// stepper XYZ
int pin_Xenable      = 22;
int pin_Xms1         = 24;
int pin_Xstep        = 26;
int pin_Xdir         = 28;
int pin_Yenable      = 30;
int pin_Yms1         = 32;
int pin_Ystep        = 34;
int pin_Ydir         = 36;
int pin_Zenable      = 38;
int pin_Zms1         = 40;
int pin_Zstep        = 42;
int pin_Zdir         = 44;

// dc motor
int pin_dcA          = 50;
int pin_dcB          = 52;

// dc button
int pin_dcButton     = 29;
int dcButtonValue    = 0;

// led
int pin_stepperLed   = 23;
int pin_autoLed      = 25;

// auto-drive
int pin_autoButton   = 27;
int autoButtonValue  = 0;
bool runAutoDrive    = true;


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

  // button
  pinMode(pin_autoButton, INPUT);
  pinMode(pin_dcButton, INPUT);

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

  digitalWrite(pin_Xenable, HIGH); // LOW:enable, HIGH:disable
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
  readButton();

  controlXYZ();
  controlDC();

  autoDrive();

  //monitor();
}



void monitor() {
  Serial.print("Pot: "); Serial.print(potValue);
  Serial.print(" | X: "); Serial.print(posX);
  Serial.print(" | Y: "); Serial.print(posY);
  Serial.print(" | Z: "); Serial.print(posZ);
  Serial.print(" | ButtonDC: "); Serial.print(dcButtonValue);
  Serial.println(" _");
}

void readPot() {
  potValue = analogRead(pin_pot);
}
void readJoystick() {
  posX = analogRead(pin_joyX);
  posY = analogRead(pin_joyY);
  posZ = analogRead(pin_joyZ);
}
void readButton() {
  dcButtonValue = digitalRead(pin_dcButton);
  autoButtonValue = digitalRead(pin_autoButton);
}
void controlLed(int pin, bool on) { // on:true, off:false
  digitalWrite(pin, on ? HIGH : LOW);
}

void controlXYZ() {

  int stepDelay = (int)potValue / 21 + 1; // calculate stepper delay here, values: 1-49
  //Serial.println(stepDelay);

  if      (posX <490)  { driveXYZ('X', stepDelay, true); } //  X:left
  else if (posX >535)  { driveXYZ('X', stepDelay, false); } // X:right
  else if (posY <490)  { driveXYZ('Y', stepDelay, false); } // Y:down
  else if (posY >535)  { driveXYZ('Y', stepDelay, true); } // Y:up
  else if (posZ >535)  { driveXYZ('Z', stepDelay, true); } // Z:up
  else if (posZ <490)  { driveXYZ('Z', stepDelay, false); } // Z:down
  else                 { stopXYZ(); }

}
void driveXYZ(char axis, int stepDelay, bool clockwise) {

  int _pin_enable = -1;
  int _pin_ms     = -1;
  int _pin_dir    = -1;
  int _pin_step   = -1;

  switch (axis) {
    case 'X':
      _pin_enable = pin_Xenable;
      _pin_ms     = pin_Xms1;
      _pin_dir    = pin_Xdir;
      _pin_step   = pin_Xstep;
      break;
    case 'Y':
      _pin_enable = pin_Yenable;
      _pin_ms     = pin_Yms1;
      _pin_dir    = pin_Ydir;
      _pin_step   = pin_Ystep;
      break;
    case 'Z':
      _pin_enable = pin_Zenable;
      _pin_ms     = pin_Zms1;
      _pin_dir    = pin_Zdir;
      _pin_step   = pin_Zstep;
      break;
  }
  
  controlLed(pin_stepperLed, true);

  // enable driver (LOW:enable, HIGH:disable)
  digitalWrite(_pin_enable, LOW); delayMicroseconds(5);

  // set direction
  digitalWrite(_pin_dir, clockwise ? LOW : HIGH); delayMicroseconds(5);

  // set step mode (LOW:full-step, HIGH:half-step)
  digitalWrite(_pin_ms, LOW); delayMicroseconds(5);

  // drive !
  digitalWrite(_pin_step, HIGH);
    if (stepDelay < 100) { delay(stepDelay); } else { delayMicroseconds(stepDelay); }
  digitalWrite(_pin_step, LOW);
    if (stepDelay < 100) { delay(stepDelay); } else { delayMicroseconds(stepDelay); }
}
void stopXYZ() {
  digitalWrite(pin_Xenable, HIGH);
  digitalWrite(pin_Yenable, HIGH);
  digitalWrite(pin_Zenable, HIGH);

  digitalWrite(pin_Xms1, LOW);
  digitalWrite(pin_Yms1, LOW);
  digitalWrite(pin_Zms1, LOW);

  digitalWrite(pin_Xstep, LOW);
  digitalWrite(pin_Ystep, LOW);
  digitalWrite(pin_Zstep, LOW);

  digitalWrite(pin_Xdir, LOW);
  digitalWrite(pin_Ydir, LOW);
  digitalWrite(pin_Zdir, LOW);

  controlLed(pin_stepperLed, false);
}


void controlDC() {
  if (dcButtonValue == HIGH) { driveDC(); }
  else                       { stopDC(); }
}
void driveDC() {
  digitalWrite(pin_dcA, HIGH);
  digitalWrite(pin_dcB, LOW);
}
void stopDC() {
  digitalWrite(pin_dcA, LOW);
  digitalWrite(pin_dcB, LOW);
}

void autoDrive() {
  //if (!runAutoDrive) { return; } // to run one-time only
  if (autoButtonValue == LOW) { return; }

  runAutoDrive = false;
  // 400:1 rotation fullstep
  int _xMove      = 400;
  int _yMove      = 20;
  int _zMoveDown  = 40;
  int _zMoveUp    = 100;
  int _yLoop      = 10;
  int _wait       = 500; // milliseconds
  int _delay      = 6000; // microseconds

  Serial.println("auto-start");
  controlLed(pin_autoLed, true);

  driveDC();

  // enable driver (LOW:enable, HIGH:disable)
  digitalWrite(pin_Xenable, LOW); delayMicroseconds(5);
  digitalWrite(pin_Yenable, LOW); delayMicroseconds(5);
  digitalWrite(pin_Zenable, LOW); delayMicroseconds(5);

  // set step mode (LOW:full-step, HIGH:half-step)
  digitalWrite(pin_Xms1, LOW); delayMicroseconds(5);
  digitalWrite(pin_Yms1, LOW); delayMicroseconds(5);
  digitalWrite(pin_Zms1, LOW); delayMicroseconds(5);
  
  // Y:direction
  digitalWrite(pin_Ydir, HIGH);

  // Z:down
  digitalWrite(pin_Zdir, HIGH);
  for (int i = 0; i < _zMoveDown; i++) {
    digitalWrite(pin_Zstep, HIGH); delayMicroseconds(_delay);
    digitalWrite(pin_Zstep, LOW); delayMicroseconds(_delay);
  }

  for (int j = 0; j < _yLoop; j++) {
    delay(_wait);

    // X:move right
    digitalWrite(pin_Xdir, LOW);
    for (int i = 0; i < _xMove; i++) {
      digitalWrite(pin_Xstep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Xstep, LOW); delayMicroseconds(_delay);
    }
    delay(_wait);

    // Y:move forward
    for (int i = 0; i < _yMove; i++) {
      digitalWrite(pin_Ystep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Ystep, LOW); delayMicroseconds(_delay);
    }
    delay(_wait);

    // X:move left
    digitalWrite(pin_Xdir, HIGH);
    for (int i = 0; i < _xMove; i++) {
      digitalWrite(pin_Xstep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Xstep, LOW); delayMicroseconds(_delay);
    }
    delay(_wait);

    // Y:move forward
    for (int i = 0; i < _yMove; i++) {
      digitalWrite(pin_Ystep, HIGH); delayMicroseconds(_delay);
      digitalWrite(pin_Ystep, LOW); delayMicroseconds(_delay);
    }
  }

  delay(_wait);

  // X:move right
  digitalWrite(pin_Xdir, LOW);
  for (int i = 0; i < _xMove; i++) {
    digitalWrite(pin_Xstep, HIGH); delayMicroseconds(_delay);
    digitalWrite(pin_Xstep, LOW); delayMicroseconds(_delay);
  }

  delay(_wait);
  
  // Y:move backward
  digitalWrite(pin_Ydir, HIGH);
  for (int i = 0; i < _yMove*_yLoop; i++) {
    digitalWrite(pin_Ystep, HIGH); delayMicroseconds(_delay);
    digitalWrite(pin_Ystep, LOW); delayMicroseconds(_delay);
  }

  delay(_wait);

  /*
  // Z:up
  digitalWrite(pin_Zdir, LOW);
  for (int i = 0; i < _zMoveUp; i++) {
    digitalWrite(pin_Zstep, HIGH); delayMicroseconds(_delay);
    digitalWrite(pin_Zstep, LOW); delayMicroseconds(_delay);
  }
  */

  stopXYZ();

  stopDC();

  Serial.println("auto-stop");
  controlLed(pin_autoLed, false);
}
