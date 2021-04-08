void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  int notes[] = {493, 440, 415, 440, 415, 369, 415, 329, 369, 311, 329, 311, 329, 311, 329};
int pin=8;
int n = 0;
  while (n < 16){
  tone(pin,notes[n],100,0);
  delay(110);
  tone(294,100,0);
  delay(110);
  n=n+1;
  }
}
