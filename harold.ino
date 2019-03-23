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
	/*while (true) {
		Serial.println("BNO055 Calibrating");

		uint8_t sys, gy, acl, mg = -1;
		bno.getCalibration(&sys, &gy, &acl, &mg);

		if (sys == 3 && gy == 3) {
		break;
		}
		flashDelay(100);
		}*/
}

//read gyro values
Attitude readGyro() {
	imu::Vector<3> vec = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
	Attitude att(vec.x(), vec.y(), vec.z());
	return att;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Servo back;
Servo front;

void runMotors(int back_val, int front_val) {
	back.writeMicroseconds(back_val + 1000);
	front.writeMicroseconds(front_val + 1000);
}

void initMotors() {
	digitalWrite(LED_BUILTIN, LOW);
	back.attach(PB1);
	front.attach(PA10);
	delay(2600);


	back.writeMicroseconds(1000);
	front.writeMicroseconds(1000);
	delay(2600);

	for(int i = 1000; i < 1500; i += 10) {
		back.writeMicroseconds(i);
		front.writeMicroseconds(i);
	}
	digitalWrite(LED_BUILTIN, HIGH);
	delay(2600);

	for(int i = 1490; i > 1000; i -= 10) {
		back.writeMicroseconds(i);
		front.writeMicroseconds(i);
	}
	digitalWrite(LED_BUILTIN, LOW);
	delay(2600);

	runMotors(300, 300);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(2000);
	runMotors(0, 0);
	digitalWrite(LED_BUILTIN, LOW);
}




//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
unsigned long lastTimeStamp;

float error;
float prev_error;
float pitch_p;
float pitch_i;
float max_i = 300;
float pitch_d;
float pid_output;


void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	//setup bno055 gyro/accel
	initIMU();
	while(!bno.begin()) {
		delay(200);
		digitalWrite(LED_BUILTIN, LOW);
	}
	digitalWrite(LED_BUILTIN, HIGH);

	//setup motors
	initMotors();


	delay(3000);
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
	//float desiredPitch = (double) map(ch2.pulseWidth, 1000, 2000, -360, 360);
	float desiredPitch = 0.0;

	error = desiredPitch - gyro.pitch;

	pitch_p = error * 0.5;

	pitch_i = pitch_i + (error * 0.005);
	if(pitch_i > max_i) pitch_i = max_i;
	if(pitch_i < -max_i) pitch_i = -max_i;

	pitch_d = (error - prev_error) * 0.05;

	pid_output = pitch_p + pitch_i + pitch_d;

	runMotors(pid_output, -pid_output);


	prev_error = error;
}







