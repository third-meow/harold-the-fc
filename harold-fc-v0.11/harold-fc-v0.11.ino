#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

sensors_event_t IMUData;
const int MAXTILT = 45;
const int MAXYAW = 150;
const int MAXTHROTTLE = 50;
const int MAXERRORS = MAXYAW + (2 * MAXTILT) + MAXTHROTTLE;
const int MINERRORS = -MAXERRORS + MAXTHROTTLE;

const int MINMOTOR = 0;
const int MAXMOTOR = 0;
struct Ryp {
  int pitch;
  int roll;
  int yaw;
  int throttle;
} reciverData;

struct Ch {
  int pin;
  volatile bool pulse;
  volatile unsigned long endPulse;
  volatile unsigned long startPulse;
  volatile int pulseWidth;
  int lastPulseWidth;
} ch1, ch2, ch3, ch4, ch5, ch6;


bool armState;
int modeState;

struct gain {
  float P;
  float I;
  float D;
} pitchGain, rollGain, yawGain;

float prvGyroX = 0;
float prvGyroY = 0;
float prvGyroZ = 0;

int prvReciverPitch = 0;
int prvReciverYaw = 0;
int prvReciverRoll = 0;

float errorPitchP;
float errorPitchI;
float errorPitchD;
float errorPitch;
float prvErrorPitchI;

float errorRollP;
float errorRollI;
float errorRollD;
float errorRoll;
float prvErrorRollI;

float errorYawP;
float errorYawI;
float errorYawD;
float errorYaw;
float prvErrorYawI;

int forLeft;
int forRight;
int backLeft;
int backRight;

Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  Serial.begin(500000);
  initReciver();
  initIMU();
}

void loop() {
  unsigned long timer = micros();
  updateIMU();
  updateReciver(&reciverData.throttle, &reciverData.yaw, &reciverData.pitch, &reciverData.roll, &modeState, &armState);
  doPID();
  sendMotors();
  //Serial.print(micros()-timer);
  delay(100);
  //Serial.print("\t");
  Serial.print(reciverData.throttle);
  Serial.print("\t");
  Serial.print(reciverData.pitch);
  Serial.print("\t");
  Serial.print(reciverData.yaw);
  Serial.print("\t");
  Serial.print(reciverData.roll);
  Serial.print("\t");
  Serial.print(modeState);
  Serial.print("\t");
  Serial.println(armState);
}


