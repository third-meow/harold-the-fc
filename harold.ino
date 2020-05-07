#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "Attitude.h"

#define LOOP_TIME 4500
#define LED_BUILTIN PC13

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
    flashDelay(60);
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

const float MAX_I = 85; // should prob be 45;
const float STAB_MAX_I = 15;

float yaw_error;
float yaw_p;

float stab_pitch_error;
float stab_prev_pitch_error = 0.0;
float stab_pitch_p;
float stab_pitch_i;
float stab_pitch_d;
float stab_pitch_pid;

float stab_roll_error;
float stab_prev_roll_error = 0.0;
float stab_roll_p;
float stab_roll_i;
float stab_roll_d;
float stab_roll_pid;

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

  // show we're alive
  for (int i = 0; i < 40; ++i) flashDelay(50);

  // setup bno055 gyro/accel
  initIMU();
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

  if (micros() > (lastTimeStamp + LOOP_TIME)) {		// Should remove this before serious flight testing
    while (true) {
      flashDelay(4000);

      setLeftMotor(0);
      setRightMotor(0);
      setFrontMotor(0);
      setBackMotor(0);
    }
  }

  // wait for full loop time
  while (micros() < (lastTimeStamp + LOOP_TIME)) {}
  lastTimeStamp = micros();

  digitalWrite(LED_BUILTIN, HIGH);
  
  //float desired_pitch = 0.0;
  float desired_roll = 0.0;
  float desired_yaw = 0.0;
  float desired_pitch = (float) map(ch2.pulseWidth, 1000, 2000, -10, 10);
  //float desired_roll = (float) map(ch1.pulseWidth, 1000, 2000, -20, 20);
  //float desired_yaw = (float) map(ch4.pulseWidth, 1000, 2000, -10, 10;

  /*
   * these are in radians / second !!
   */
  gyro = readGyro();
  /*
   * these are in degrees !!
   */
  euler = readOrientation();

  stab_pitch_error = -(desired_pitch - euler.pitch);
  stab_pitch_p = stab_pitch_error * 0.9; //0.9 .. seems ..okay? still gets wobbly tho
  stab_pitch_i = stab_pitch_i + (stab_pitch_error * 0.005);
  if (stab_pitch_i > STAB_MAX_I) {
    stab_pitch_i = STAB_MAX_I;
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (stab_pitch_i < -STAB_MAX_I){
    stab_pitch_i = -STAB_MAX_I;
    digitalWrite(LED_BUILTIN, LOW);

  }
  stab_pitch_d = (stab_pitch_error - stab_prev_pitch_error) * 0.8; // could up this even more?
  stab_pitch_pid = stab_pitch_p + stab_pitch_i + stab_pitch_d;
  stab_prev_pitch_error = stab_pitch_error;

  stab_roll_error = -(desired_roll - euler.roll);
  stab_roll_p = stab_roll_error * 0.9;//0.9 .. seems ..okay? still gets wobbly tho
  stab_roll_i = stab_roll_i + (stab_roll_error * 0.005);
  if (stab_roll_i > STAB_MAX_I) {
    stab_roll_i = STAB_MAX_I;
  }
  if (stab_roll_i < -STAB_MAX_I) {
    stab_roll_i = -STAB_MAX_I;
  }
  stab_roll_d = (stab_roll_error - stab_prev_roll_error) * 0.8; // could up this even more? 
  stab_roll_pid = stab_roll_p + stab_roll_i + stab_roll_d;
  stab_prev_roll_error = stab_roll_error;
  

  pitch_error = stab_pitch_pid - gyro.pitch;
  // RATE P GAIN SHOULD BE:
  // WHEN FLYING: 0.8
  // WHEN IN RIG: 1.2
  pitch_p = pitch_error * 0.8;
  pitch_i = pitch_i + (pitch_error * 0.01);
  if (pitch_i > MAX_I) pitch_i = MAX_I;
  if (pitch_i < -MAX_I) pitch_i = -MAX_I;
  pitch_d = (pitch_error - prev_pitch_error) * 0.4;
  pitch_pid = pitch_p + pitch_i + pitch_d;
  prev_pitch_error = pitch_error;

  roll_error = stab_roll_pid - gyro.roll;
    // RATE P GAIN SHOULD BE:
  // WHEN FLYING: 0.8
  // WHEN IN RIG: 1.2
  roll_p = roll_error * 0.8;
  roll_i = roll_i + (roll_error * 0.01);
  if (roll_i > MAX_I) roll_i = MAX_I;
  if (roll_i < -MAX_I) roll_i = -MAX_I;
  roll_d = (roll_error - prev_roll_error) * 0.4;
  roll_pid = roll_p + roll_i + roll_d;
  prev_roll_error = roll_error;

  yaw_error = desired_yaw - gyro.yaw;
  yaw_p = yaw_error * 0.7;
  


  float throt = (float) map(ch3.pulseWidth, 1000, 2000, -5, 450);
  //float throt = 100;


  if (throt > 0) {
    setLeftMotor(throt + roll_pid - yaw_p);
    setRightMotor(throt - roll_pid - yaw_p);
    setFrontMotor(throt + pitch_pid + yaw_p);
    setBackMotor(throt - pitch_pid + yaw_p);
  } else {

    //reset intergrals
    pitch_i = 0;
    roll_i = 0;

    stab_pitch_i = 0;
    stab_roll_i = 0;
    
    setLeftMotor(0);
    setRightMotor(0);
    setFrontMotor(0);
    setBackMotor(0);
  }

}


