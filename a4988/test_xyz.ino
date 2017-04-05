int pin_Xstep  = 3;
int pin_Xdir   = 4;
int pin_Ystep  = 6;
int pin_Ydir   = 7;
int pin_Zstep  = 8;
int pin_Zdir   = 9;

int d = 700;
int w = 100;

void setup() {
  pinMode(pin_Xstep,OUTPUT);
  pinMode(pin_Xdir,OUTPUT);
  pinMode(pin_Ystep,OUTPUT);
  pinMode(pin_Ydir,OUTPUT);
  pinMode(pin_Zstep,OUTPUT);
  pinMode(pin_Zdir,OUTPUT);

  digitalWrite(pin_Xstep,LOW);
  digitalWrite(pin_Xdir,LOW);
  digitalWrite(pin_Ystep,LOW);
  digitalWrite(pin_Ydir,LOW);
  digitalWrite(pin_Zstep,LOW);
  digitalWrite(pin_Zdir,LOW);
}
void loop() {
  // 400: 1 rotation
  digitalWrite(pin_Xdir,LOW);    
  for(int x = 0; x < 400; x++) {
    digitalWrite(pin_Xstep,HIGH);
      delayMicroseconds(d);
    digitalWrite(pin_Xstep,LOW);
      delayMicroseconds(d);
  }
  delay(w);
  
  digitalWrite(pin_Ydir,LOW);
  for(int y = 0; y < 400; y++) {
    digitalWrite(pin_Ystep,HIGH);
      delayMicroseconds(d);
    digitalWrite(pin_Ystep,LOW);
      delayMicroseconds(d);
  }
  delay(w);
  
  digitalWrite(pin_Zdir,LOW);
  for(int z = 0; z < 400; z++) {
    digitalWrite(pin_Zstep,HIGH);
      delayMicroseconds(d);
    digitalWrite(pin_Zstep,LOW);
      delayMicroseconds(d);
  }
  delay(w);
}
