// pins
int x_step = 30;
int x_dir  = 31;
int y_step = 32;
int y_dir  = 33;
int z_step = 34;
int z_dir  = 35;

int msec=100; // microseconds

void setup() {
  pinMode(x_dir, OUTPUT);
  pinMode(x_step, OUTPUT);
  pinMode(y_dir, OUTPUT);
  pinMode(y_step, OUTPUT);
  pinMode(z_dir, OUTPUT);
  pinMode(z_step, OUTPUT);
  
  digitalWrite(x_dir, LOW);
  digitalWrite(x_step, LOW);
  digitalWrite(y_dir, LOW);
  digitalWrite(y_step, LOW);
  digitalWrite(z_dir, LOW);
  digitalWrite(z_step, LOW);
}

void loop() {
  digitalWrite(x_step, HIGH);
    delayMicroseconds(msec);
  digitalWrite(x_step, LOW);
    delayMicroseconds(msec);
  
  digitalWrite(y_step, HIGH);
    delayMicroseconds(msec);
  digitalWrite(y_step, LOW);
    delayMicroseconds(msec);
  
  digitalWrite(z_step, HIGH);
    delayMicroseconds(msec);
  digitalWrite(z_step, LOW);
    delayMicroseconds(msec);
}
