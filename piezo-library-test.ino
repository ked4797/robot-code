#include <Tone32.h>

#define pin 32
#define BUZZER_CHANNEL 0

void setup() {
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  int notes[] = {493, 440, 415, 440, 415, 369, 415, 329, 369, 311, 329, 311, 329, 311, 329,311};
int n = 0;
  while (n < 16){
  tone(pin,notes[n],110,0);
  tone(pin, 294,110,0);
  n=n+1;
  }
}
