int pin_K_ENABLE = 2;
int pin_K_DIR    = 4;
int pin_K_STEP   = 3;

int pin_L_ENABLE = 5;
int pin_L_DIR    = 7;
int pin_L_STEP   = 6;

int stepCount  = 400; // 400:1 rotation in full-step mode
bool clockwise = true;


void setup() {
  pinMode(pin_K_ENABLE, OUTPUT);
  pinMode(pin_K_DIR, OUTPUT);
  pinMode(pin_K_STEP, OUTPUT);
  
  pinMode(pin_L_ENABLE, OUTPUT);
  pinMode(pin_L_DIR, OUTPUT);
  pinMode(pin_L_STEP, OUTPUT);

  digitalWrite(pin_K_ENABLE, HIGH); // LOW:enable,HIGH:disable
  digitalWrite(pin_K_DIR, LOW); // LOW:clockwise, HIGH:anti-clockwise
  digitalWrite(pin_K_STEP, LOW);

  digitalWrite(pin_L_ENABLE, HIGH); // LOW:enable,HIGH:disable
  digitalWrite(pin_L_DIR, LOW); // LOW:clockwise, HIGH:anti-clockwise
  digitalWrite(pin_L_STEP, LOW);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(pin_K_ENABLE, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_L_ENABLE, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pin_K_DIR, clockwise?LOW:HIGH);
  delayMicroseconds(2);
  digitalWrite(pin_L_DIR, clockwise?LOW:HIGH);
  delayMicroseconds(2);

  if(clockwise){clockwise=false;} else {clockwise=true;}
  
  for(int i=0; i<stepCount; i++){
    //Serial.print(i);    
    digitalWrite(pin_K_STEP, HIGH);	
    delayMicroseconds(2);
    digitalWrite(pin_L_STEP, HIGH);
    delayMicroseconds(700);
    digitalWrite(pin_K_STEP, LOW);
	delayMicroseconds(2);
	digitalWrite(pin_L_STEP, LOW);
    delayMicroseconds(700);
  }

  digitalWrite(pin_K_ENABLE, HIGH);
  digitalWrite(pin_K_DIR, LOW);
  digitalWrite(pin_K_STEP, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_L_ENABLE, HIGH);
  digitalWrite(pin_L_DIR, LOW);
  digitalWrite(pin_L_STEP, LOW);

  Serial.println("stop...");
  delay(500);  
}
