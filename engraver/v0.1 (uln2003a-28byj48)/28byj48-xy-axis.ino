/*
	10-03-2017
*/

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

// stepper Y
int pinY_blue=8;
int pinY_pink=9;
int pinY_yellow=10;
int pinY_orange=11;
int stepperYstep=-1;
int stepperYdelay=2;



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
  
  pinMode(pinY_blue,OUTPUT);
  pinMode(pinY_pink,OUTPUT);
  pinMode(pinY_yellow,OUTPUT);
  pinMode(pinY_orange,OUTPUT);

  digitalWrite(pinX_blue,LOW);
  digitalWrite(pinX_pink,LOW);
  digitalWrite(pinX_yellow,LOW);
  digitalWrite(pinX_orange,LOW);
  
  digitalWrite(pinY_blue,LOW);
  digitalWrite(pinY_pink,LOW);
  digitalWrite(pinY_yellow,LOW);
  digitalWrite(pinY_orange,LOW);
}

void loop() {
  delay(2);
  
  readPot();
  readJoystick();
  controlStepperX();
  controlStepperY();
  
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
  Serial.print(stepperXstep);  
  Serial.print(" | stepY: ");
  Serial.println(stepperYstep);
}





void controlStepperX(){
  bool clockwise=true;
  bool runStepper=false;
  
  if(xPos<500) {clockwise=true; runStepper=true;}
  if(xPos>524) {clockwise=false; runStepper=true;}

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

void controlStepperY(){
  bool clockwise=true;
  bool runStepper=false;
  
  if(yPos<500) {clockwise=true; runStepper=true;}
  if(yPos>524) {clockwise=false; runStepper=true;}

  if(runStepper){
    if (stepperYstep==-1){ if(clockwise){stepperYstep=0;} else {stepperYstep=3;} }
    else if(stepperYstep==0){ if(clockwise){stepperYstep=1;} else {stepperYstep=3;} }
    else if(stepperYstep==1){ if(clockwise){stepperYstep=2;} else {stepperYstep=0;} }
    else if(stepperYstep==2){ if(clockwise){stepperYstep=3;} else {stepperYstep=1;} }
    else if(stepperYstep==3){ if(clockwise){stepperYstep=0;} else {stepperYstep=2;} }

    // set speed:
    stepperYdelay = (int)(potValue/50);
    
    driveStepperY();
    delay(stepperYdelay);
  }
  else{
    stopStepperY();
  }
}
void driveStepperY(){
  switch(stepperYstep){
    case 0:
      digitalWrite(pinY_blue,HIGH);
      digitalWrite(pinY_pink,LOW);
      digitalWrite(pinY_yellow,LOW);
      digitalWrite(pinY_orange,LOW);
      break;
    case 1:
      digitalWrite(pinY_blue,LOW);
      digitalWrite(pinY_pink,HIGH);
      digitalWrite(pinY_yellow,LOW);
      digitalWrite(pinY_orange,LOW);
      break;
    case 2:
      digitalWrite(pinY_blue,LOW);
      digitalWrite(pinY_pink,LOW);
      digitalWrite(pinY_yellow,HIGH);
      digitalWrite(pinY_orange,LOW);
      break;
    case 3:
      digitalWrite(pinY_blue,LOW);
      digitalWrite(pinY_pink,LOW);
      digitalWrite(pinY_yellow,LOW);
      digitalWrite(pinY_orange,HIGH);
      break;
  }  
}
void stopStepperY(){
  digitalWrite(pinY_blue,LOW);
  digitalWrite(pinY_pink,LOW);
  digitalWrite(pinY_yellow,LOW);
  digitalWrite(pinY_orange,LOW);
}
