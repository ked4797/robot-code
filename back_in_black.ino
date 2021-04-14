void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int notes[] = {493, 440, 164, 293, 277, 246, 277, 195, 246, 195, 220, 246, 233};
int pin=8;
int n = 0;
while (n < 16){
  tone(pin,notes[n],100);
  delay(110);
  n=n+1;
}
}
// This is very broken
