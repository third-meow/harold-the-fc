#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define MAXROLL 45
#define MAXPITCH 45

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

//current and previous pitch, yaw, roll and thottle values from reciver
struct Pytr {
  int pitch;
  int roll;
  int yaw;
  int throttle;
} reciverData, prvReciverData;

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
}rateGains, stabGains;  //gains for rate and stabilisation PID

//holds P, I, D & PID errors for pitch, yaw and roll
struct Error {
  float pitch[3];
  float prvPitch_I;
  float yaw[3];
  float prvYaw_I;
  float roll[3];
  float prvRoll_I;
}rateErrors, stabErrors;  //errors for both rate and stabilisation PIDs

//holds if built-in LED is off or on
bool ledState = false;

//setup BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  //start serial at 500,000 buad
  Serial.begin(500000);
  //initiate reciver & inertial measurement unit
  initReciver();
  initIMU();
  //set all PID gains
  setGains();

  //setup LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  //allow everything a second to start
  delay(1000);
}

void loop(){
  //read reciver
  updateReciver(&reciverData.throttle, &reciverData.yaw, &reciverData.pitch, &reciverData.roll, &modeState, &armState);

  Serial.print(ch1.pulseWidth);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(ch2.pulseWidth);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(ch3.pulseWidth);
  Serial.print("\t");
  Serial.print("\t");
  Serial.println(ch4.pulseWidth);

  delay(200);
}


