int pin_ENABLE = 11;
int pin_DIR    = 2;
int pin_STEP   = 3;
int pin_MS1    = 10;
int pin_MS2    = 9;
int pin_MS3    = 8;
int stepCount  = 400; // 400:1 rotation in full-step mode



void setup() {
  pinMode(pin_ENABLE, OUTPUT);
  pinMode(pin_DIR, OUTPUT);
  pinMode(pin_STEP, OUTPUT);
  pinMode(pin_MS1, OUTPUT);
  pinMode(pin_MS2, OUTPUT);
  pinMode(pin_MS3, OUTPUT);

  digitalWrite(pin_ENABLE, HIGH); // LOW:enable,HIGH:disable
  digitalWrite(pin_DIR, LOW); // LOW:clockwise, HIGH:anti-clockwise
  digitalWrite(pin_STEP, LOW);
  digitalWrite(pin_MS1, LOW);
  digitalWrite(pin_MS2, LOW);
  digitalWrite(pin_MS3, LOW);

  Serial.begin(9600);
}

void loop() {
  Serial.println("starting..");
  delay(2000);
  
  setStep(1); drive(); stop();
  
  setStep(2); drive(); stop();
  
  setStep(3); drive(); stop();
  
  setStep(4); drive(); stop();
  
  setStep(5); drive(); stop();
}

void setStep(int s){
  switch (s){
    case 1:
      Serial.print("full step...");
      digitalWrite(pin_MS1, LOW);
      digitalWrite(pin_MS2, LOW);
      digitalWrite(pin_MS3, LOW);
      break;
    case 2:
      Serial.print("half step...");
      digitalWrite(pin_MS1, HIGH);
      digitalWrite(pin_MS2, LOW);
      digitalWrite(pin_MS3, LOW);
      break;
    case 3:
      Serial.print("quarter step...");
      digitalWrite(pin_MS1, LOW);
      digitalWrite(pin_MS2, HIGH);
      digitalWrite(pin_MS3, LOW);
      break;
    case 4:
      Serial.print("8th step...");
      digitalWrite(pin_MS1, HIGH);
      digitalWrite(pin_MS2, HIGH);
      digitalWrite(pin_MS3, LOW);
      break;
    case 5:
      Serial.print("16th step...");
      digitalWrite(pin_MS1, HIGH);
      digitalWrite(pin_MS2, HIGH);
      digitalWrite(pin_MS3, HIGH);
      break;
  }
  
}

void drive(){
  digitalWrite(pin_ENABLE, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_DIR, LOW);
  
  for(int i=0; i<stepCount; i++){
    //Serial.print(i);    
    digitalWrite(pin_STEP, HIGH);
    delayMicroseconds(700);
    digitalWrite(pin_STEP, LOW);
    delayMicroseconds(700);
  }
}

void stop(){
  digitalWrite(pin_ENABLE, HIGH);
  digitalWrite(pin_DIR, LOW);
  digitalWrite(pin_STEP, LOW);

  Serial.println("stop...");
  delay(2000);
}
