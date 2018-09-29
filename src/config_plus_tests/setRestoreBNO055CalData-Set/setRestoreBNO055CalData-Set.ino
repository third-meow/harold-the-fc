#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>

//holds the calibration data from the IMU
adafruit_bno055_offsets_t godwitCalData;

//setup BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  Serial.begin();
  pinMode(PC13, OUTPUT);
  delay(5000);

  logIMUOffsets();
}

void loop() {
  digitalWrite(PC13, HIGH);
  delay(500);
  digitalWrite(PC13, LOW);
  delay(500);
}
