#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "Attitude.h"

#define LOOP_TIME 4000
#define LED_BUILTIN PC13

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Adafruit_BNO055 bno(19, 0x29);
Attitude gyro;

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

    if (gy == 3) {
      break;
    }
  }
}

// read gyro values (in radians / second !!)
Attitude readGyro() {
  /*
   * these are all in radians / second !!
   */
  imu::Vector<3> vec = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  Attitude att(vec.x(), vec.y(), vec.z());
  return att;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

unsigned long lastTimeStamp;

float max_i = 85; // should prob be 45;

float yaw_error;
float yaw_p;

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


  /*
   * these are all in radians / second !!
   */
  gyro = readGyro();
  float desired_pitch = 0.0;
  float desired_roll = 0.0;
  float desired_yaw = 0.0;
  //float desired_pitch = (float) map(ch2.pulseWidth, 1000, 2000, -50, 50);
  //float desired_roll = (float) map(ch1.pulseWidth, 1000, 2000, -50, 50);
  //float desired_yaw = (float) map(ch4.pulseWidth, 1000, 2000, -50, 50);
  


  digitalWrite(LED_BUILTIN, HIGH);

  pitch_error = desired_pitch - gyro.pitch;
  pitch_p = pitch_error * 1.2;
  pitch_i = pitch_i + (pitch_error * 0.01);
  if (pitch_i > max_i) {
    pitch_i = max_i;
    digitalWrite(LED_BUILTIN, LOW);
  } else if (pitch_i < -max_i) {
    pitch_i = -max_i;
    digitalWrite(LED_BUILTIN, LOW);
  }
  pitch_d = (pitch_error - prev_pitch_error) * 0.4; //0.4 seems good
  pitch_pid = pitch_p + pitch_i + pitch_d;
  prev_pitch_error = pitch_error;

  roll_error = desired_roll - gyro.roll;
  roll_p = roll_error * 1.2;
  roll_i = roll_i + (roll_error * 0.01);
  if (roll_i > max_i) {
    roll_i = max_i;
    digitalWrite(LED_BUILTIN, LOW);
  } else if (roll_i < -max_i) {
    roll_i = -max_i;
    digitalWrite(LED_BUILTIN, LOW);
  }
  roll_d = (roll_error - prev_roll_error) * 0.4;
  roll_pid = roll_p + roll_i + roll_d;
  prev_roll_error = roll_error;

  yaw_error = desired_yaw - gyro.yaw;
  yaw_p = yaw_error * 0.0;


  // map throttle
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
    
    setLeftMotor(0);
    setRightMotor(0);
    setFrontMotor(0);
    setBackMotor(0);
  }

}


