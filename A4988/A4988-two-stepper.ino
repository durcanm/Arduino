/*
 * mdurcan 2017-10-25
 *  
 * pin_ENABLE > LOW:enable,HIGH:disable
 * pin_DIR > LOW:clockwise, HIGH:anti-clockwise
 */

int pin_A_ENABLE   = 2;
int pin_A_DIR      = 3;
int pin_A_STEP     = 4;

int pin_B_ENABLE   = 5;
int pin_B_DIR      = 6;
int pin_B_STEP     = 7;

int stepCount      = 1200; // 400:1 rotation in full-step mode
bool clockwise     = true; // true:1, false:0

int stepDelay      = 700; // microseconds


void setup() {
  pinMode(pin_A_ENABLE, OUTPUT);
  pinMode(pin_A_DIR, OUTPUT);
  pinMode(pin_A_STEP, OUTPUT);
  
  pinMode(pin_B_ENABLE, OUTPUT);
  pinMode(pin_B_DIR, OUTPUT);
  pinMode(pin_B_STEP, OUTPUT);

  digitalWrite(pin_A_ENABLE, HIGH);
  digitalWrite(pin_A_DIR, LOW);
  digitalWrite(pin_A_STEP, LOW);
  
  digitalWrite(pin_B_ENABLE, HIGH);
  digitalWrite(pin_B_DIR, LOW);
  digitalWrite(pin_B_STEP, LOW);

  Serial.begin(9600);
}

void loop() {
  Serial.print("clockwise: ");
  Serial.println(clockwise);
  
  driveA();   stop();  pause();
  driveB();   stop();  pause();
  driveAB();  stop();  pause();

  if(clockwise){clockwise=false;} else {clockwise=true;}
}

void driveA() {
  Serial.print("drive > A");
  
  digitalWrite(pin_A_ENABLE, LOW);
  digitalWrite(pin_A_DIR, clockwise?LOW:HIGH);
  
  for(int i=0; i<stepCount; i++){
    digitalWrite(pin_A_STEP, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(pin_A_STEP, LOW);
    delayMicroseconds(stepDelay);
  }
}

void driveB() {
  Serial.print("drive > B");
  
  digitalWrite(pin_B_ENABLE, LOW);
  digitalWrite(pin_B_DIR, clockwise?LOW:HIGH);
  
  for(int i=0; i<stepCount; i++){
    digitalWrite(pin_B_STEP, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(pin_B_STEP, LOW);
    delayMicroseconds(stepDelay);
  }
}

void driveAB() {
  Serial.print("drive > A-B");
  
  digitalWrite(pin_A_ENABLE, LOW);
  digitalWrite(pin_B_ENABLE, LOW);
  digitalWrite(pin_A_DIR, clockwise?LOW:HIGH);
  digitalWrite(pin_B_DIR, clockwise?LOW:HIGH);
  
  for(int i=0; i<stepCount; i++){
    digitalWrite(pin_A_STEP, HIGH);
    digitalWrite(pin_B_STEP, HIGH);
    delayMicroseconds(stepDelay);
    
    digitalWrite(pin_A_STEP, LOW);
    digitalWrite(pin_B_STEP, LOW);
    delayMicroseconds(stepDelay);
  }
}

void stop() {
  digitalWrite(pin_A_ENABLE, HIGH);
  digitalWrite(pin_A_DIR, LOW);
  digitalWrite(pin_A_STEP, LOW);
  
  digitalWrite(pin_B_ENABLE, HIGH);
  digitalWrite(pin_B_DIR, LOW);
  digitalWrite(pin_B_STEP, LOW);

  Serial.println("....stop");
}

void pause() {
   delay(1000); // milliseconds
}
