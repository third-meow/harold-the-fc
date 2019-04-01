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
  volatile long startPulse;  //to hold pulse start time volatile
  long pulseWidth;  //to hold pulse width in microseconds
} ch1, ch2, ch3, ch4, ch5, ch6;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void flashDelay(int len) {
  digitalWrite(LED_BUILTIN, LOW);
  delay(len / 2);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(len / 2);
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

Servo back;
Servo front;
Servo left;
Servo right;
void setFrontMotor(int val) {
  front.writeMicroseconds(val + 1000);
}

void setBackMotor(int val) {
  back.writeMicroseconds(val + 1000);
}

void setLeftMotor(int val) {
  left.writeMicroseconds(val + 1000);
}

void setRightMotor(int val) {
  right.writeMicroseconds(val + 1000);
}

void initMotors() {
  digitalWrite(LED_BUILTIN, LOW);
  back.attach(PB1);
  front.attach(PA10);
  left.attach(PA9);
  right.attach(PA8);
  delay(2600);


  back.writeMicroseconds(1000);
  front.writeMicroseconds(1000);
  left.writeMicroseconds(1000);
  right.writeMicroseconds(1000);
  delay(2600);

  for (int i = 1000; i < 1500; i += 10) {
    back.writeMicroseconds(i);
    front.writeMicroseconds(i);
    left.writeMicroseconds(i);
    right.writeMicroseconds(i);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2600);

  for (int i = 1490; i > 1000; i -= 10) {
    back.writeMicroseconds(i);
    front.writeMicroseconds(i);
    left.writeMicroseconds(i);
    right.writeMicroseconds(i);
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(2600);

  setBackMotor(100);
  setFrontMotor(100);
  setLeftMotor(100);
  setRightMotor(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  setBackMotor(0);
  setFrontMotor(0);
  setLeftMotor(0);
  setRightMotor(0);
  digitalWrite(LED_BUILTIN, LOW);
}




//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
unsigned long lastTimeStamp;

float max_i = 33;

float pitch_error;
float prev_pitch_error = 0.0;
float pitch_p;
float pitch_i;
float pitch_d;
float pitch_pid;

float roll_error;
float prev_roll_error = 0.0;
float roll_p;
float roll_i;
float roll_d;
float roll_pid;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //setup bno055 gyro/accel
  initIMU();
  while (!bno.begin()) {
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  digitalWrite(LED_BUILTIN, HIGH);

  //setup receiver
  initReceiver();
  //setup motors
  initMotors();


  delay(3000);
  lastTimeStamp = micros();

  digitalWrite(LED_BUILTIN, HIGH);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


void loop() {

  if (micros() > (lastTimeStamp + LOOP_TIME)) {
    Serial.println("TOO SLOW, LOOP TIME MET!. THIS IS BAD!");
    while (true) {
      flashDelay(4000);

      setLeftMotor(0);
      setRightMotor(0);
      setFrontMotor(0);
      setBackMotor(0);
    }
  }

  //wait for full loop time
  while (micros() < (lastTimeStamp + LOOP_TIME)) {}
  lastTimeStamp = micros();

  gyro = readGyro();
  //float desired_pitch = (float) map(ch2.pulseWidth, 1000, 2000, -50, 50);
  //float desired_roll = (float) map(ch1.pulseWidth, 1000, 2000, -50, 50);
  float desired_roll = 0.0;
  float desired_pitch = 0.0;


  pitch_error = desired_pitch - gyro.pitch;
  pitch_p = pitch_error * 1.16;
  pitch_i = pitch_i + (pitch_error * 0.003);
  if (pitch_i > max_i) pitch_i = max_i;
  if (pitch_i < -max_i) pitch_i = -max_i;
  pitch_d = (pitch_error - prev_pitch_error) * 0.26;
  pitch_pid = pitch_p + pitch_i + pitch_d;
  prev_pitch_error = pitch_error;

  roll_error = desired_roll - gyro.roll;
  roll_p = roll_error * 1.16;
  roll_i = roll_i + (roll_error * 0.003);
  if (roll_i > max_i) roll_i = max_i;
  if (roll_i < -max_i) roll_i = -max_i;
  roll_d = (roll_error - prev_roll_error) * 0.26;
  roll_pid = roll_p + roll_i + roll_d;
  prev_roll_error = roll_error;


	// map throttle. 
	float throt = (float) map(ch3.pulseWidth, 1000, 2000, -5, 220);
	
	if (throt > 0) {
		setLeftMotor(throt + roll_pid);
		setRightMotor(throt - roll_pid);
		setFrontMotor(throt + pitch_pid);
		setBackMotor(throt - pitch_pid);
	} else {
		setLeftMotor(0);
		setRightMotor(0);
		setFrontMotor(0);
		setBackMotor(0);
	}

}






