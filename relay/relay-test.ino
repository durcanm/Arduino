int pin_relay1 = 36;
int pin_relay2 = 38;
int pin_relay3 = 40;
int pin_relay4 = 42;

void setup()
{ 
  pinMode(pin_relay1, OUTPUT);
  pinMode(pin_relay2, OUTPUT);
  pinMode(pin_relay3, OUTPUT);
  pinMode(pin_relay4, OUTPUT);
  
  digitalWrite(pin_relay1, LOW);
  digitalWrite(pin_relay2, LOW);
  digitalWrite(pin_relay3, LOW);
  digitalWrite(pin_relay4, LOW);
}

void loop()
{
  delay(100);
  
  digitalWrite(pin_relay1, HIGH); delay(1000);
  digitalWrite(pin_relay1, LOW); delay(1000);  
  
  digitalWrite(pin_relay2, HIGH); delay(1000);
  digitalWrite(pin_relay2, LOW); delay(1000);
  
  digitalWrite(pin_relay3, HIGH); delay(1000);
  digitalWrite(pin_relay3, LOW); delay(1000);
  
  digitalWrite(pin_relay4, HIGH); delay(1000);
  digitalWrite(pin_relay4, LOW); delay(1000);
  
  delay(3000);
  
  digitalWrite(pin_relay1, HIGH);
  digitalWrite(pin_relay2, HIGH);
  digitalWrite(pin_relay3, HIGH);
  digitalWrite(pin_relay4, HIGH);
  
  delay(3000);
  
  digitalWrite(pin_relay1, LOW);
  digitalWrite(pin_relay2, LOW);
  digitalWrite(pin_relay3, LOW);
  digitalWrite(pin_relay4, LOW);
  
  delay(3000);
  
}
