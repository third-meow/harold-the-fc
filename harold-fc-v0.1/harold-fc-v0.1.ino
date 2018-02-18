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

struct Pry {
  int pitch;
  int roll;
  int yaw;
} gyroData, desiredCMD;

struct Trea {
  int throttle;
  int rudder;
  int elevator;
  int aileron;
} reciverData;




void setup() {}

void loop() {
  updateGyro(&gyroData.pitch, &gyroData.roll, &gyroData.yaw);
  updateReciver(&reciverData.throttle, &reciverData.rudder, &reciverData.elevator, &reciverData.aileron);
  
}


void updateGyro(int *pitch, int *roll, int *yaw) {}
void updateReciver(int *throttle, int *rudder, int *elevator, int *aileron) {}

void doPID(int pitch, int roll, int yaw, int *dPitch, int *dRoll, int *dYaw) {}


