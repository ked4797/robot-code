// robot code version two, now with actual functioning code and things
// GitHub would make this whole versioning thing much easier, just saying...

/* TO-DO: check if code will run if 'sensors' keyword is changed (for temp sensor)
          add motor code
          make this do useful things
          */

// basically, it's a first attempt at combining all of the sensors and it didn't go that well I will improve it 

#include <Wire.h>
#include <VL53L0X.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "I2Cdev.h"
#include "MPU6050.h"

VL53L0X tof;
#define TOF1 23
#define TOF2 13
#define TOF3 2
#define TOF4 5
#define IR 14
#define loudspeaker 26
#define tracker1 27
#define tracker2 4
#define temp 25
#define motor1 16
#define motor2 17
#define motor3 18
#define motor4 19

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
bool blinkState = false;

IRrecv irrecv(IR);
decode_results results;

OneWire oneWire(temp);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(38400);
  // possibly not necessary, part of the gyro setup
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif


    
  Wire.begin();
  // both of the tracker sensors are inputs and will be low when they are not line-following
  pinMode(tracker1, INPUT_PULLDOWN);
  pinMode(tracker2, INPUT_PULLDOWN);

  // not 100% sure how this will work with multiple TOFs, I can't test since I only have one
  tof.setTimeout(500);
  if (!tof.init())
  {
    Serial.println("Failed to detect and initialize TOF sensor!");
    while (1) {}
  }

  // IR receiver setup
  irrecv.enableIRIn();

  // temperature sensor setup
  sensors.begin();

  // start gyro
  accelgyro.initialize(); 
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}






void loop() {
  // reads from a TOF once
  // past about 1.2m, the sensor decides everything is 8190mm away
  Serial.print("TOF reading: ");
  Serial.println(tof.readRangeSingleMillimeters());

  // outputs whatever the IR sensor is receiving, if anything
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    Serial.print("IR sensor reading: ");
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    irrecv.resume();  // Receive the next value
  }

  // used to request the temperature from all sensors on the OneWire bus
  // this does not work
  // neither does the example code
  // there is also a *mild* possibility that I mighta sorta kinda fried it but in my defence it didn't work beforehand anyway
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }



  // reading from gyro 
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  #ifdef OUTPUT_READABLE_ACCELGYRO
      // display tab-separated accel/gyro x/y/z values
      Serial.print("a/g:\t");
      Serial.print(ax); Serial.print("\t");
      Serial.print(ay); Serial.print("\t");
      Serial.print(az); Serial.print("\t");
      Serial.print(gx); Serial.print("\t");
      Serial.print(gy); Serial.print("\t");
      Serial.println(gz);
  #endif

  #ifdef OUTPUT_BINARY_ACCELGYRO
      Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
      Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
      Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
      Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
      Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
      Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
  #endif`   `
  
}
