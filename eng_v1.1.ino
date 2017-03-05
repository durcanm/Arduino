// pot
int pinPot=A5;
int potValue=0; // 0-1023

// joystick XY
int pinX=A0;
int pinY=A1;
int pinButton=12;
int xPos=0;
int yPos=0;
int buttonState=0;

// stepper X
int pinX_blue=2;
int pinX_pink=3;
int pinX_yellow=4;
int pinX_orange=5;
int stepperXstep=-1;
int stepperXdelay=2;



void setup() {
  Serial.begin(9600);

  pinMode(pinPot,INPUT);
  pinMode(pinX,INPUT);
  pinMode(pinY,INPUT);
  pinMode(pinButton,INPUT_PULLUP); //activate pull-up resistor on the push-button pin
  
  pinMode(pinX_blue,OUTPUT);
  pinMode(pinX_pink,OUTPUT);
  pinMode(pinX_yellow,OUTPUT);
  pinMode(pinX_orange,OUTPUT);

  digitalWrite(pinX_blue,LOW);
  digitalWrite(pinX_pink,LOW);
  digitalWrite(pinX_yellow,LOW);
  digitalWrite(pinX_orange,LOW);
}

void loop() {
  delay(2);
  
  readPot();
  readJoystick();
  controlStepperX();
  
  //monitor();
}


void readPot(){
  potValue = analogRead(pinPot);
}
void readJoystick(){
  xPos = analogRead(pinX);
  yPos = analogRead(pinY);
  buttonState = digitalRead(pinButton);
}
void monitor(){
  Serial.print("Pot: ");
  Serial.print(potValue);
  Serial.print(" | X: ");
  Serial.print(xPos);
  Serial.print(" | Y: ");
  Serial.print(yPos);
  Serial.print(" | Button: ");
  Serial.print(buttonState);
  Serial.print(" _");
  
  Serial.print(" | stepX: ");
  Serial.println(stepperXstep);
}





void controlStepperX(){
  bool clockwise=true;
  bool runStepper=false;
  
  if(yPos<500) {clockwise=true; runStepper=true;}
  if(yPos>524) {clockwise=false; runStepper=true;}

  if(runStepper){
    if (stepperXstep==-1){ if(clockwise){stepperXstep=0;} else {stepperXstep=3;} }
    else if(stepperXstep==0){ if(clockwise){stepperXstep=1;} else {stepperXstep=3;} }
    else if(stepperXstep==1){ if(clockwise){stepperXstep=2;} else {stepperXstep=0;} }
    else if(stepperXstep==2){ if(clockwise){stepperXstep=3;} else {stepperXstep=1;} }
    else if(stepperXstep==3){ if(clockwise){stepperXstep=0;} else {stepperXstep=2;} }

    // set speed:
    stepperXdelay = (int)(potValue/50);
    
    driveStepperX();
    delay(stepperXdelay);
  }
  else{
    stopStepperX();
  }
}
void driveStepperX(){
  switch(stepperXstep){
    case 0:
      digitalWrite(pinX_blue,HIGH);
      digitalWrite(pinX_pink,LOW);
      digitalWrite(pinX_yellow,LOW);
      digitalWrite(pinX_orange,LOW);
      break;
    case 1:
      digitalWrite(pinX_blue,LOW);
      digitalWrite(pinX_pink,HIGH);
      digitalWrite(pinX_yellow,LOW);
      digitalWrite(pinX_orange,LOW);
      break;
    case 2:
      digitalWrite(pinX_blue,LOW);
      digitalWrite(pinX_pink,LOW);
      digitalWrite(pinX_yellow,HIGH);
      digitalWrite(pinX_orange,LOW);
      break;
    case 3:
      digitalWrite(pinX_blue,LOW);
      digitalWrite(pinX_pink,LOW);
      digitalWrite(pinX_yellow,LOW);
      digitalWrite(pinX_orange,HIGH);
      break;
  }  
}
void stopStepperX(){
  digitalWrite(pinX_blue,LOW);
  digitalWrite(pinX_pink,LOW);
  digitalWrite(pinX_yellow,LOW);
  digitalWrite(pinX_orange,LOW);
}
