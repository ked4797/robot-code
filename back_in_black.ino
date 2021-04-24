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
  int notes[] = {164, 293, 277, 246, 277, 195, 246, 220, 246, 233, 246, 246};
  int notes2[] = {220, 391, 369, 329, 369, 261, 329, 293, 329, 311, 329, 329};
  int times[] = {163, 163, 163, 163, 163, 326, 163, 326, 163, 326, 163, 326};
int n = 0;
int x = 0;
while (x < 3){
while (n < 12){
  tone(pin,notes[n],times[n],0);
  n=n+1;
  }
x=x+1;
n=0;
}
x=0;
while (x < 1){
while (n < 12){
  tone(pin,notes2[n],times[n],0);
  n=n+1;
}
x=x+1;
n=0;
}
}
