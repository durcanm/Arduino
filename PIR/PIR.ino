////////
//Simple PIR Sensor sketch 
//http://www.youtube.com/educ8s

int ledPin = 13;
int pirPin = 9;
int val = 0;

void setup() 
{
  pinMode (ledPin,OUTPUT);
  pinMode (pirPin, INPUT);
}
void loop () 
{
  val = digitalRead(pirPin);
  digitalWrite(ledPin,val);

if (val == 1)
  digitalWrite(ledPin,LOW);
}
