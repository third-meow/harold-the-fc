#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

const int MAXTILT = 45;

bool armState;
int modeState;

struct Ch {
  int pin;
  volatile bool pulse;
  volatile unsigned long endPulse;
  volatile unsigned long startPulse;
  volatile int pulseWidth;
  int pulseWidth;
} ch1, ch2, ch3, ch4, ch5, ch6;

struct Ryp {
  int pitch;
  int roll;
  int yaw;
  int throttle;
} reciverData, desiredCMD;

/*
struct Xyz {
  int x;
  int y;
  int z;
} accelData;
*/

Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  Serial.begin(500000);
  initReciver();
  initIMU();
}

void loop() {
  updateIMU();
  updateReciver(&reciverData.throttle, &reciverData.yaw, &reciverData.pitch, &reciverData.roll, &modeState, &armState);
  doPID();
}


