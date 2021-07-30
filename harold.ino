#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "Attitude.h"

#define LOOP_TIME 4500
#define LED_BUILTIN PC13

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Servo front;
void setFrontMotor(int val) {
  val += 1000;
  front.writeMicroseconds(min(max(val, 1000), 2000));
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Adafruit_BNO055 bno(19, 0x29);
imu::Vector<3> vec;
Attitude gyro;
Attitude euler;

struct PWMinput {
  volatile long startPulse;  // to hold pulse start time volatile
  long pulseWidth;  // to hold pulse width in microseconds
} ch1, ch2, ch3, ch4, ch5, ch6;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void flashDelay(int len) {
  digitalWrite(LED_BUILTIN, LOW);
  delay(len / 2);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(len / 2);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// setup bno055
void initIMU() {
  while (!bno.begin()) {
    delay(20);
  }

  bno.setExtCrystalUse(true);
  while (true) {
    flashDelay(200);
    uint8_t sys, gy, acl, mg;
    sys = gy = acl = mg = 0;
    bno.getCalibration(&sys, &gy, &acl, &mg);

    if (gy == 3 && acl >= 2) {
      break;
    }
  }
}

// read gyro values (in radians / second !!)
Attitude readGyro() {
  /*
   * these are all in radians / second !!
   */
  vec = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  Attitude att(vec.x(), vec.y(), vec.z());
  return att;
}

// read orentation values (in degrees)
Attitude readOrientation() {
  vec = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  Attitude att(vec.z(), vec.y(), vec.x());
  return att;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

unsigned long lastTimeStamp;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // show we're alive
	flashDelay(4000);

  // setup bno055 gyro/accel
  initIMU();

	flashDelay(70);

  // setup motor
  digitalWrite(LED_BUILTIN, LOW);
  front.attach(PA10);
  delay(2600);
  front.writeMicroseconds(1000);
  delay(2600);
  for (int i = 1000; i < 1500; i += 10) {
    front.writeMicroseconds(i);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2600);
  for (int i = 1490; i > 1000; i -= 10) {
    front.writeMicroseconds(i);
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(2600);
  setFrontMotor(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  setFrontMotor(0);
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  lastTimeStamp = micros();

  digitalWrite(LED_BUILTIN, HIGH);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


void loop() {
	// wait for full loop time
	while (micros() < (lastTimeStamp + LOOP_TIME)) {}
	lastTimeStamp = micros();

	digitalWrite(LED_BUILTIN, HIGH);

	gyro = readGyro(); //these are in rads^-1

	if (gyro.pitch > 2.) {
		setFrontMotor(0);
		delay(5);
	}
	else {
		setFrontMotor(100);
	}
}
