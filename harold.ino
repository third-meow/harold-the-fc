#include<Wire.h>
#include<Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "Attitude.h"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Adafruit_BNO055 bno(19, 0x29);
Attitude gyro;

struct PWMinput {
  volatile long startPulse;  //to hold pulse start time
  volatile long pulseWidth;  //to hold pulse width in microseconds
  
} ch1, ch2, ch3, ch4, ch5, ch6;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void flashDelay(int len) {
  digitalWrite(LED_BUILTIN, LOW);
  delay(len/2);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(len/2);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//setup bno055
void initIMU() {
  while (bno.begin() == false) {
    Serial.println("BNO055 not found");
    flashDelay(200);
  }

  bno.setExtCrystalUse(true);
  while (true) {
    Serial.println("BNO055 Calibrating");

    uint8_t sys, gy, acl, mg = -1;
    bno.getCalibration(&sys, &gy, &acl, &mg);

    if (sys == 3 && gy == 3) {
      break;
    }

    flashDelay(100);
  }
}

//read gyro values
Attitude readGyro() {
  imu::Vector<3> vec = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
	Attitude att(vec.x(), vec.y(), vec.z());
	return att;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //setup bno055 gyro/accel
  initIMU();

	//setup RC receiver
	initReceiver();
  
  digitalWrite(LED_BUILTIN, HIGH);


}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void loop() {
  Serial.print(1000);
  Serial.print("\t");
  Serial.print(2000);
  Serial.print("\t");
  Serial.print(ch1.pulseWidth);
  Serial.print("\n");
  delay(20);
}





