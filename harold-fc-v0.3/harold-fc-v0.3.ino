/*
   Gyro data ---------------------------------> pid ----------.
                                                 ^            |
                                                 |            v
                                                 |    convert to motor speed
                                                 |                     |
                                                 |                     v
                                                 |                   motors
   Reciver data --------> angular data ----------'
*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

const int MAXTILT = 45;

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
int ch[] = {A0, A1, A2, A3};

Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  initReciver();
  initIMU();
}

void loop() {
  updateIMU();
  updateReciver(&reciverData.throttle, &reciverData.yaw, &reciverData.pitch, &reciverData.roll);

}




void doPID(int pitch, int roll, int yaw, int *dPitch, int *dRoll, int *dYaw) {}


