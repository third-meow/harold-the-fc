#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//gyro data (degrees/s)
imu::Vector<3> gyro;

//create BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

//holds gains and maxs
struct Gain {
  float pitch[2];
  float yaw[2];
  float roll[2];
  uint16_t pitchMax[3];
  uint16_t yawMax[3];
  uint16_t rollMax[3];
} rateGains; //gains for rate PID

//holds PID errors
struct Error {
  float pitch[3];
  float yaw[3];
  float roll[3];
} rateErrors; //errors for rate PID

//loop time variables
uint32_t lastLoopStart;
uint16_t loopGoal = 4000;

uint32_t lastSlowLoopStart;
uint16_t slowLoopGoal = 30000;

void setup() {
  Serial.begin();
  Serial.println("Intergral Test");

  if (!initIMU()) {
    Serial.println("NO BNO055 FOUND");
  }

  //set all PID gains
  setGains();

  lastSlowLoopStart = micros();
  lastLoopStart = micros();
}

void loop() {
  if (micros() - lastLoopStart >= loopGoal) {

    lastLoopStart = micros();

    updateIMU();

    doPIDs();
  }
  if (micros() - lastSlowLoopStart >= slowLoopGoal){
    
    lastSlowLoopStart = micros();

    Serial.print(rateErrors.roll[0]); // real P
    Serial.print("\t");
    Serial.print(rateErrors.roll[1]); // real I
    Serial.print("\t");
    Serial.print((0 - gyro.y()) * 0.025); //goal mix
    Serial.print("\t");
    Serial.println(rateErrors.roll[0] + rateErrors.roll[1]); //real mix
  }
}
