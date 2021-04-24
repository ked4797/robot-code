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
  int notes[] = {329, 587, 554, 493, 554, 391, 493, 440, 493, 466, 493, 493};
  int times[] = {163, 163, 163, 163, 163, 326, 163, 326, 163, 326, 163, 326};
int pin=8;
int n = 0;
while (n < 12){
  tone(pin,notes[n],times[n],0);
  n=n+1;
}
}
