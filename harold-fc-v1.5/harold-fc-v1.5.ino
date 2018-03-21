#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define RC_CH1_MIN 1000
#define RC_CH1_MAX 1997
#define RC_CH2_MIN 998
#define RC_CH2_MAX 1991
#define RC_CH3_MIN 997
#define RC_CH3_MAX 1995
#define RC_CH4_MIN 991
#define RC_CH4_MAX 1985
#define RC_CH5_MIN 994
#define RC_CH5_MAX 1988
#define RC_CH6_MIN 999
#define RC_CH6_MAX 2000

#define MAXROLL 45
#define MAXPITCH -45

//*sbrk() used for FreeRam() calculations
extern "C" char *sbrk(int i);

//calculates how much free ram is left
int FreeRam () {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}

//gyro data (degrees/s)
imu::Vector<3> gyro;
//orientation data (degrees)
imu::Vector<3> euler;
//previous gyro data (degrees/s)
imu::Vector<3> prvGyro;
//previous orientation data (degrees)
imu::Vector<3> prvEuler;

//current and previous pitch, yaw, roll and thottle values from receiver
struct Pytr {
  int8_t pitch;
  int8_t roll;
  int16_t yaw;
  uint16_t throttle;
} receiverData, prvReceiverData;

int16_t yawTarget = 0;

//has every variable you need for a PWM input
struct PWMinput {
  int8_t pin;  //the input pin
  volatile bool pulse;  //the pulse "state" (during pulse or waiting for pulse)
  volatile uint32_t startPulse;  //to hold pulse start time in micros since reset
  volatile uint32_t endPulse;  //to hold pulse end time in micros since reset
  volatile uint16_t pulseWidth;  //to hold pulse width in microseconds
  uint16_t prvPulseWidth; //to hold previous pulse width
} ch1, ch2, ch3, ch4, ch5, ch6;   //create 6 channels

bool armState = false; //starts unarmed
uint8_t modeState; //holds mode (0,1,2)

//manual yaw gain used when yaw not locked, to turn on the yaw axis at the correct pace
float manualYawGain;

//holds gains
struct Gain {
  float pitch[2];
  float yaw[2];
  float roll[2];
} rateGains, stabGains; //gains for rate and stabilisation PID

//holds P, I, D & PID errors for pitch, yaw and roll
struct Error {
  float pitch[3];
  float prvPitch_I;
  float yaw[3];
  float prvYaw_I;
  float roll[3];
  float prvRoll_I;
} rateErrors, stabErrors, prvStabErrors; //errors for both rate and stabilisation PIDs

//setup BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

Servo frontLeftMotor;
Servo frontRightMotor;
Servo backLeftMotor;
Servo backRightMotor;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  //start serial at 500,000 buad
  Serial.begin(500000);

  for (int i = 0; i < 4000; i++) {
    delay(1);
    if (Serial) {
      Serial.println("Initializing");
      break;
    }
  }

  //initiate receiver, motors & inertial measurement unit
  Serial.println("initializing Reciver");
  if (!initReceiver()) {
    Serial.println("Reciver initalization error");
    while (true) {}
  }

  Serial.println("initializing IMU");
  if (!initIMU()) {
    Serial.println("IMU initalization error");
    while (true) {}
  }
  
  //set all PID gains
  setGains();

  Serial.println("Waiting on arming signal");
  while (armState == false) {
    updateReceiver(&receiverData.throttle,
                   &receiverData.yaw,
                   &receiverData.pitch,
                   &receiverData.roll,
                   &modeState,
                   &armState);
    delay(1000);
  }
  Serial.println("About to Arm Motors!");
  if (!initMotors()) {
    Serial.println("Motor initalization error");
    while (true) {}
  }
  //allow everything a second to start
  delay(1000);

}

void loop() {
  //read inertial measurement unit
  updateIMU();
  //read RC receiver
  updateReceiver(&receiverData.throttle, &receiverData.yaw, &receiverData.pitch, &receiverData.roll, &modeState, &armState);
  //run PIDS
  doPIDs();

  //if throttle is not below 50
  if (receiverData.throttle >= (RC_CH3_MIN + 50)) {
    //send data from PIDS to motors
    sendMotors();
  } else {
    //send off signal to motors
    stopMotors();
    //keep yawTarget from "locking" by reseting
    resetYawTarget();
  }

}


