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

//inertial measurement unit data (has gyro data in)
sensors_event_t IMUData;
//previous inertial measurement unit data
sensors_event_t prvIMUData;

//current and previous pitch, yaw, roll and thottle values from receiver
struct Pytr {
  int pitch;
  int roll;
  int yaw;
  int throttle;
} receiverData, prvReceiverData;

//has every variable you need for a PWM input
struct PWMinput {
  int pin;  //the input pin
  volatile bool pulse;  //the pulse "state" (during pulse or waiting for pulse)
  volatile unsigned long startPulse;  //to hold pulse start time in micros since reset
  volatile unsigned long endPulse;  //to hold pulse end time in micros since reset
  volatile int pulseWidth;  //to hold pulse width in microseconds
  int prvPulseWidth; //to hold previous pulse width
} ch1, ch2, ch3, ch4, ch5, ch6;   //create 6 channels

bool armState = false; //starts unarmed
byte modeState; //holds mode (0,1,2)

//holds gains
struct Gain {
  byte pitch[2];
  byte yaw[2];
  byte roll[2];
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

//holds if built-in LED is off or on
bool ledState = false;

//setup BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  //start serial at 500,000 buad
  Serial.begin(500000);
  //initiate receiver & inertial measurement unit
  initReceiver();
  initIMU();
  //set all PID gains
  setGains();

  //setup LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //allow everything a second to start
  delay(1000);
}

void loop() {
  //read RC receiver
  updateReceiver(&receiverData.throttle, &receiverData.yaw, &receiverData.pitch, &receiverData.roll, &modeState, &armState);

  Serial.print(receiverData.throttle);
  Serial.print("\t\t");
  Serial.print(receiverData.yaw);
  Serial.print("\t\t");
  Serial.print(receiverData.pitch);
  Serial.print("\t\t");
  Serial.print(receiverData.roll);
  Serial.print("\t");
  Serial.print(modeState);
  Serial.print("\t");
  Serial.println(armState);
}


