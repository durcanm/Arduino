int pin_relay1 = 5;
int pin_relay2 = 6;
int pin_relay3 = 7;
int pin_relay4 = 8;
int pin_relay5 = 9;

void setup()
{ 
  pinMode(pin_relay1, OUTPUT);
  pinMode(pin_relay2, OUTPUT);
  pinMode(pin_relay3, OUTPUT);
  pinMode(pin_relay4, OUTPUT);
  pinMode(pin_relay5, OUTPUT);
  
  digitalWrite(pin_relay1, LOW);
  digitalWrite(pin_relay2, LOW);
  digitalWrite(pin_relay3, LOW);
  digitalWrite(pin_relay4, LOW);
  digitalWrite(pin_relay5, LOW);
}

void loop()
{
  int _delay = 3000;
  
  delay(100);
  
  digitalWrite(pin_relay1, HIGH); delay(_delay);
  digitalWrite(pin_relay1, LOW); delay(_delay);  
  
  digitalWrite(pin_relay2, HIGH); delay(_delay);
  digitalWrite(pin_relay2, LOW); delay(_delay);
  
  digitalWrite(pin_relay3, HIGH); delay(_delay);
  digitalWrite(pin_relay3, LOW); delay(_delay);
  
  digitalWrite(pin_relay4, HIGH); delay(_delay);
  digitalWrite(pin_relay4, LOW); delay(_delay);
  
  digitalWrite(pin_relay5, HIGH); delay(_delay);
  digitalWrite(pin_relay5, LOW); delay(_delay);
  
  delay(_delay);
  
  digitalWrite(pin_relay1, HIGH);
  digitalWrite(pin_relay2, HIGH);
  digitalWrite(pin_relay3, HIGH);
  digitalWrite(pin_relay4, HIGH);
  digitalWrite(pin_relay5, HIGH);
  
  delay(_delay);
  
  digitalWrite(pin_relay1, LOW);
  digitalWrite(pin_relay2, LOW);
  digitalWrite(pin_relay3, LOW);
  digitalWrite(pin_relay4, LOW);
  digitalWrite(pin_relay5, LOW);
  
  delay(_delay);
  
}
