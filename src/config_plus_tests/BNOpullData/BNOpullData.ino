#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

byte address = 0x29;

Adafruit_BNO055 godwit = Adafruit_BNO055(19, address);
adafruit_bno055_offsets_t godwitCalData;

void setup() {
  Serial.begin();

  if(godwit.begin() == false){
    Serial.println("No BNO055 Found");
  }else{
    godwit.setExtCrystalUse(true);
    while(godwit.isFullyCalibrated() == false){
      delay(300);
    }
    godwit.getSensorOffsets(godwitCalData);

    displaySensorOffsets(godwitCalData);
  }
}

void loop() {
  delay(2000);
}


void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.print("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.println(" ");

    Serial.print("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.println(" ");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.println(" ");

    Serial.print("\nAccel Radius: ");
    Serial.println(calibData.accel_radius);

    Serial.println("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
}
