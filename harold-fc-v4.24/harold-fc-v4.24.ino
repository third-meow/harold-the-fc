#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define SERIAL_OUT false

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

#define LED_INSIDE PC13

#define MAXROLL 60
#define MAXPITCH -60

//*sbrk() used for FreeRam() calculations
extern "C" char *sbrk(int i);

//calculates how much free ram is left
int FreeRam () {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}

//loop time variables
uint32_t lastLoopStart;
uint16_t loopGoal = 4000;

/*
 **************************************************************************************************
 */

//gyro data (degrees/s)
imu::Vector<3> gyro;
//orientation data (degrees)
imu::Vector<3> euler;
//previous gyro data (degrees/s)
imu::Vector<3> prvGyro;
//previous orientation data (degrees)
imu::Vector<3> prvEuler;

float eulerPitchOffset = 0.0;
float eulerRollOffset = 0.0;

/*
 **************************************************************************************************
 */

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
  volatile uint32_t startPulse;  //to hold pulse start time
  volatile uint16_t pulseWidth;  //to hold pulse width in microseconds
} ch1, ch2, ch3, ch4, ch5, ch6;   //create 6 channels

bool armState = false; //starts unarmed
uint8_t modeState; //holds mode (0,1,2)

//manual yaw gain used when yaw not locked, to turn on the yaw axis at the correct pace
float manualYawGain;

/*
 **************************************************************************************************
 */

//holds gains and maxs
struct Gain {
  float pitch[2];
  float yaw[2];
  float roll[2];
  uint16_t pitchMax[3];
  uint16_t yawMax[3];
  uint16_t rollMax[3];
} rateGains, stabGains; //gains for rate and stabilisation PID

//holds P, I, D & PID errors for pitch, yaw and roll
struct Error {
  float pitch[3];
  float prvPitch_I;
  float yaw[3];
  float prvYaw_I;
  float roll[3];
  float prvRoll_I;
} rateErrors, stabErrors; //errors for both rate and stabilisation PIDs

/*
 **************************************************************************************************
 */

//setup BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

Servo frontLeftMotor;
Servo frontRightMotor;
Servo backLeftMotor;
Servo backRightMotor;


void setup() {
  Serial.begin();
  Serial3.begin(9600);
  serialStartup();

  pinMode(LED_INSIDE, OUTPUT);
  writeLED(false);

  initReceiver();
  
  if (!initIMU()){
    Serial.println("NO BNO055 FOUND");
  }

  //set all PID gains
  setGains();

  while (armState == false) {
    updateReceiver(&receiverData.throttle,
                   &receiverData.yaw,
                   &receiverData.pitch,
                   &receiverData.roll,
                   &modeState,
                   &armState);
    delay(1000);
  }

  //wait for IMU to calibrate
  while (getIMUCalStatus() == false){
    delay(500);
  }

  //set IMU "level" 
  setIMUlevel();
  
  //start motors
  initMotors();

  //allow everything a second to start
  delay(1000);

  lastLoopStart = micros() + loopGoal;
}

void loop() {
  if (micros() - lastLoopStart >= loopGoal) {

    lastLoopStart = micros();

    //read inertial measurement unit
    updateIMU();

    //read RC receiver
    updateReceiver(&receiverData.throttle, &receiverData.yaw, &receiverData.pitch, &receiverData.roll, &modeState, &armState);

    //run PIDS
    doPIDs();

    String outLog = "";

    if (int(stabErrors.pitch[3]) < 9){
      outLog.concat(int(stabErrors.pitch[3]));
      outLog.concat(" ");
    }else{
      outLog.concat(int(stabErrors.pitch[3]));
    }
    outLog.concat(" | ");//--------------------------

    if (int(rateErrors.pitch[3]) < 9){
      outLog.concat(int(rateErrors.pitch[3]));
      outLog.concat(" ");
    }else{
      outLog.concat(int(rateErrors.pitch[3]));
    }
    outLog.concat(" | ");//--------------------------

    if (int(stabErrors.roll[3]) < 9){
      outLog.concat(int(stabErrors.roll[3]));
      outLog.concat(" ");
    }else{
      outLog.concat(int(stabErrors.roll[3]));
    }
    outLog.concat(" | ");//-------------------------

    if (int(rateErrors.roll[3]) < 9){
      outLog.concat(int(rateErrors.roll[3]));
      outLog.concat(" ");
    }else{
      outLog.concat(int(rateErrors.roll[3]));
    }

    Serial3.println(outLog);
    
    

    //if throttle is not below 20
    if (receiverData.throttle >= (RC_CH3_MIN + 20)) {
      //send data from PIDS to motors
      sendMotors();
    } else {
      //send off signal to motors
      stopMotors();
      //keep yawTarget from "locking" by reseting
      resetYawTarget();
      //reset intergrals
      resetI();
    }
  }
}


