int bluePin = 8;
int pinkPin = 9;
int yellowPin = 10;
int orangePin = 11;
int currentStep = 0;

int count=0;

void setup(){
    pinMode(bluePin, OUTPUT);
    pinMode(pinkPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(orangePin, OUTPUT);
    
    digitalWrite(bluePin, LOW);
    digitalWrite(pinkPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(orangePin, LOW);
    
    Serial.begin(9600);
}

void loop(){
    if(count<2000){
    
        switch (currentStep){
            case 0:
                digitalWrite(bluePin, HIGH);
                digitalWrite(pinkPin, LOW);
                digitalWrite(yellowPin, LOW);
                digitalWrite(orangePin, LOW);
            break;
            case 1:
                digitalWrite(bluePin, LOW);
                digitalWrite(pinkPin, HIGH);
                digitalWrite(yellowPin, LOW);
                digitalWrite(orangePin, LOW);
            break;
            case 2:
                digitalWrite(bluePin, LOW);
                digitalWrite(pinkPin, LOW);
                digitalWrite(yellowPin, HIGH);
                digitalWrite(orangePin, LOW);
            break;
            case 3:
                digitalWrite(bluePin, LOW);
                digitalWrite(pinkPin, LOW);
                digitalWrite(yellowPin, LOW);
                digitalWrite(orangePin, HIGH);
            break;
        }
        
        currentStep = (++currentStep<4) ? currentStep:0;
        
        delay(2);
        
        count++;
        //Serial.println(count);
    }
}
