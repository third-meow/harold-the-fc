#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "Attitude.h"

#define LOOP_TIME 4000

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
unsigned long lastTimeStamp;

double error;
double prev_error;
double pitch_p;
double pitch_i;
double max_i = 400;
double pitch_d;
double pid_output;

  
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //setup bno055 gyro/accel
  initIMU();

	//setup RC receiver
	initReceiver();
  
  digitalWrite(LED_BUILTIN, HIGH);

	lastTimeStamp = micros();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


void loop() {
	if (micros() > (lastTimeStamp + LOOP_TIME)) {
		Serial.println("TOO SLOW, LOOP TIME MET!. THIS IS BAD!");
		while(true) {} 
	}
	//wait for full loop time
	while (micros() < (lastTimeStamp + LOOP_TIME)) {}
  lastTimeStamp = micros();

	gyro = readGyro();
	double desiredPitch = (double) map(ch2.pulseWidth, 1000, 2000, -360, 360);
	error = gyro.pitch - desiredPitch;
	pitch_p = error * 0.5;
	pitch_i = pitch_i + (error * 0.005);
	if(pitch_i > max_i) pitch_i = max_i;
	if(pitch_i < -max_i) pitch_i = -max_i;
	pitch_d = (error - prev_error) * 0.05;
	pid_output = pitch_p + pitch_i + pitch_d;

	Serial.print(-1000);
	Serial.print("\t");
	Serial.print(1000);
	Serial.print("\t");
	Serial.print(desiredPitch);
	Serial.print("\t");
  Serial.print(error);
	Serial.print("\t");
	Serial.print(pid_output);
	Serial.print("\n");

	prev_error = error;
}



