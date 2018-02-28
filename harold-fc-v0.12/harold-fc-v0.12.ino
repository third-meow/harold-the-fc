#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


extern "C" char *sbrk(int i);

int FreeRam () {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}

sensors_event_t IMUData;

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
byte modeState;

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

bool ledState = false;

Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  Serial.begin(500000);
  initReciver();
  initIMU();
  initGains();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  delay(100);

  updateIMU();
  updateReciver(&reciverData.throttle, &reciverData.yaw, &reciverData.pitch, &reciverData.roll, &modeState, &armState);
  doPID();
  sendMotors();

  Serial.print(millis()/1000);
  Serial.print("\t");
  Serial.print(IMUData.orientation.x);
  Serial.print("\t");
  Serial.print(IMUData.orientation.y);
  Serial.print("\t");
  Serial.print(IMUData.orientation.z);
  Serial.print("\t");
  Serial.println(FreeRam());
}


