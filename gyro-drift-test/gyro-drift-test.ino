#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//gyro data (degrees/s)
imu::Vector<3> gyro;
//orientation data (degrees)
imu::Vector<3> euler;
//previous gyro data (degrees/s)
imu::Vector<3> prvGyro;
//previous orientation data (degrees)
imu::Vector<3> prvEuler;

//setup BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

float z[] = {0, 0, 0, 0, 0};
float zgain[] = {0.013, 0.0112};

void setup() {
  delay(2000);
  Serial1.begin(9600);
  if (!initIMU()) {
    Serial1.println("NO BNO055 FOUND");
  }

  while (getIMUCalStatus() == false) {
    delay(500);
  }
}

void loop() {
  //read inertial measurement unit
  updateIMU();
  z[0] = gyro.z();

  z[1] = (0.00 - z[0]) * zgain[0];
  z[2] = z[2] + (0 - z[0]) * zgain[1];
  z[4] = z[1] + z[2];
  
  Serial1.print(z[0]/100);
  Serial1.print("\t");
  Serial1.println(z[4]);
  delay(40);
}
