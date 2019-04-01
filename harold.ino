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

float max_i = 43;

float yaw_error;
float yaw_p;

float pitch_error;
float prev_pitch_error = 0.0;
float pitch_i;
float pitch_pid;

float roll_error;
float prev_roll_error = 0.0;
float roll_i;
float roll_pid;

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
  while (bno.begin() == false) {
    flashDelay(200);
  }

  bno.setExtCrystalUse(true);
  while (true) {

    uint8_t sys, gy, acl, mg = -1;
    bno.getCalibration(&sys, &gy, &acl, &mg);

    if (sys == 3 && gy == 3) {
      break;
    }
    flashDelay(100);
  }
}
// read gyro values
Attitude readGyro() {
  imu::Vector<3> vec = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  Attitude att(vec.x(), vec.y(), vec.z());
  return att;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

float calc_pid(float p_gain, float i_gain, float d_gain, float err, float *prev_err, float *i) {
	// P
	float p = err * p_gain;
	// I
	*i = *i + (err * i_gain);
  if (*i > max_i) *i = max_i;
  if (*i < -max_i) *i = -max_i;
	// D
	float d = (err - *prev_err) * d_gain;
	// prev err
	*prev_err = err;

	// PID
	return (p + *i + d);
}




//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
unsigned long lastTimeStamp;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // setup bno055 gyro/accel
  initIMU();
  while (!bno.begin()) {
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  digitalWrite(LED_BUILTIN, HIGH);

  // setup receiver
  initReceiver();
  // setup motors
  initMotors();


  delay(3000);
  lastTimeStamp = micros();

  digitalWrite(LED_BUILTIN, HIGH);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


void loop() {

  if (micros() > (lastTimeStamp + LOOP_TIME)) {
    while (true) {
      flashDelay(4000);
			setAllMotors(0);
    }
  }

  // wait for full loop time
  while (micros() < (lastTimeStamp + LOOP_TIME)) {}
	// set last time stamp
  lastTimeStamp = micros();

  gyro = readGyro();
  //float desired_pitch = (float) map(ch2.pulseWidth, 1000, 2000, -50, 50);
  float desired_pitch = 0.0;
  //float desired_roll = (float) map(ch1.pulseWidth, 1000, 2000, -50, 50);
  float desired_roll = 0.0;
  //float desired_yaw = (float) map(ch4.pulseWidth, 1000, 2000, -50, 50);
  float desired_yaw = 0.0;


  pitch_error = desired_pitch - gyro.pitch;
  pitch_pid = calc_pid(1.16, 0.01,  0.26, pitch_error, &prev_pitch_error, &pitch_i);

  roll_error = desired_roll - gyro.roll;
  roll_pid = calc_pid(1.16, 0.01,  0.26, roll_error, &prev_roll_error, &roll_i);

	yaw_error = desired_yaw - gyro.yaw;
	yaw_p = yaw_error * 0.5;


	// map throttle 
	float throt = (float) map(ch3.pulseWidth, 1000, 2000, -5, 220);
	
	if (throt > 0) {
		setLeftMotor(throt + roll_pid + yaw_p);
		setRightMotor(throt - roll_pid + yaw_p);
		setFrontMotor(throt + pitch_pid - yaw_p);
		setBackMotor(throt - pitch_pid - yaw_p);
	} else {
		setAllMotors(0);
	}

}



