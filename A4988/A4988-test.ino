int pin_ENABLE = 11;
int pin_DIR    = 2;
int pin_STEP   = 3;
int pin_MS1    = 10;
int pin_MS2    = 9;
int pin_MS3    = 8;
int stepCount  = 8000; // 400:1 rotation in full-step mode

bool clockwise = true;


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
  digitalWrite(pin_ENABLE, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_DIR, clockwise?LOW:HIGH);

  if(clockwise){clockwise=false;} else {clockwise=true;}
  
  for(int i=0; i<stepCount; i++){
    //Serial.print(i);    
    digitalWrite(pin_STEP, HIGH);
    delayMicroseconds(700);
    digitalWrite(pin_STEP, LOW);
    delayMicroseconds(700);
  }

  digitalWrite(pin_ENABLE, HIGH);
  digitalWrite(pin_DIR, LOW);
  digitalWrite(pin_STEP, LOW);

  Serial.println("stop...");
  delay(500);  
}
