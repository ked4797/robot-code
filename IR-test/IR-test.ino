#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "analogWrite.h"

// IR setup
const uint16_t kRecvPin = 14;
IRrecv irrecv(kRecvPin);
decode_results results;

#define right0 17
#define right1 16
#define left0 19
#define left1 18

int rightSpeed = 207;
int leftSpeed = 204;
bool standardSpeed = true;

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }
  
  // put your setup code here, to run once:
  pinMode(right0, OUTPUT);
  pinMode(left0, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(left1, OUTPUT);

  analogWrite(left0, 0);
  analogWrite(left1, leftSpeed);
  analogWrite(right0, 0);
  analogWrite(right1, rightSpeed);

  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  // take a reading from IR
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    if (results.value == 0xFF01FE && !standardSpeed){
      // go straight on
      rightSpeed = 207;
      leftSpeed = 204;
      analogWrite(left1, leftSpeed);
      analogWrite(right1, rightSpeed);
      standardSpeed = true;
      Serial.println("Heading straight");
    }
    else if (results.value == 0xFF04FB){
      // robot is off to the left
      rightSpeed-=5;
      leftSpeed+=5;
      analogWrite(right1, rightSpeed);
      analogWrite(left1, leftSpeed);
      standardSpeed = false;
      Serial.println("Veering right");
    }
    else if (results.value == 0xFF02FD){
      // robot is off to the right
      rightSpeed+=5;
      leftSpeed-=5;
      analogWrite(right1, rightSpeed);
      analogWrite(left1, leftSpeed);
      standardSpeed = false;
      Serial.println("Veering left");
    }
    irrecv.resume();  // Receive the next value
  }
}
