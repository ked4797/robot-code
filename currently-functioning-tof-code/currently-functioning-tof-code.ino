// #1 is front
// #2 is right

#include "Adafruit_VL53L0X.h"
#include "analogWrite.h"
#include "Tone32.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 23
#define SHT_LOX2 5

#define right0 17
#define right1 16
#define left0 19
#define left1 18

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

bool turn = false;
bool turned = false;
bool done = false;
unsigned long startTime;
bool stopped = false;
bool reversing = false;
bool contact = false;
unsigned long contactStartTime;

/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

void read_dual_sensors() {

  // take readings from TOFs
  lox1.rangingTest(&measure1, true); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, true); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print(F("1: "));
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("2: "));
  if(measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.println();


  lox2.rangingTest(&measure2, true);
  // if the distance to the right is greater than 250mm or the robot is in the process of turning, and it has not turned before and it's been more than 8 seconds
   if ((measure2.RangeMilliMeter > 250 || turn == true) && turned == false && millis() > 6000){
    // if this is the first time the previous if statement has been true
    if (turn != true){
      // start turning and record the start turn time
      turn = true;
      startTime = millis();
      analogWrite(left1, 0);
      analogWrite(right1, 0);
      analogWrite(left1, 200);
      Serial.write("Turning...\n");
    }

    // if it has been more than 1.5 seconds
   if (millis() - startTime > 1300){
    // stop 
    analogWrite(left1, 0);   
    analogWrite(left1, 200);
    analogWrite(right1, 200);
    turned = true;
    }
  }


  lox1.rangingTest(&measure1, true);
  // if whatever is in front of the robot is less than 150mm away
  if (measure1.RangeMilliMeter < 150 || contact){
    Serial.println("Contact");
    // if it has not yet stopped 
    if (!contact){
      // stop and save start time
      contact = true;
      contactStartTime = millis();
      analogWrite(left1, 0);
      analogWrite(right1, 0);
    }
    // if it has been more than 0.5 seconds
    /*if (millis() - contactStartTime > 500 && !reversing){
      // reverse
      reversing = true;
      analogWrite(left0, 200);
      analogWrite(right0, 200);
    }*/   

    if (millis() - contactStartTime > 2000){
      analogWrite(right0, 0);
      analogWrite(left0, 0);
      analogWrite(left1, 200);
      analogWrite(right1, 200);
      contact = false;
      reversing = false;
    }
  }

  /*if (contact && millis() - contactStartTime > 2000){
    contact = false;
    analogWrite(left0, 0);
    analogWrite(right0, 0);
    analogWrite(left1, 200);
    analogWrite(right1, 200);
  }*/
}

// this is the first thing to happen 
void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  pinMode (right0, OUTPUT);
  pinMode (left0, OUTPUT);
  pinMode (right1, OUTPUT);
  pinMode (left1, OUTPUT);

  analogWrite(left0, 0);
  analogWrite(left1, 200);
  analogWrite(right0, 0);
  analogWrite(right1, 200);
  Serial.println("Motors on");

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  
  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();
 
}

void loop() {
   
  read_dual_sensors();
  delay(100);
}
