bool logIMUOffsets() {
  if (godwit.begin()) {
    delay(200);
    godwit.setExtCrystalUse(true);

    //setup EEPROM
    EEPROM.PageBase0 = 0x801F000;
    EEPROM.PageBase1 = 0x801F800;
    EEPROM.PageSize  = 0x400;
    EEPROM.init();
    EEPROM.format();

    //wait for sensor to calibrate
    Serial.println("Waiting for IMU to calibrate");
    while (godwit.isFullyCalibrated() == false) {
      delay(100);
    }

    //take the calibration data
    Serial.println("Recording IMU calibration Data");
    godwit.getSensorOffsets(godwitCalData);

    Serial.println("Storing IMU calibration Data");

    /*
       The offsets will be useing these addresses

       - Accel  X  | 0x10 -
       - Accel  Y  | 0x11 -
       - Accel  Z  | 0x12 -
       - Gyro   X  | 0x13 -
       - Gyro   Y  | 0x14 -
       - Gryo   Z  | 0x15 -
       - Accel Rad | 0x16 -
    */

    EEPROM.write(0x10, godwitCalData.accel_offset_x);
    EEPROM.write(0x11, godwitCalData.accel_offset_y);
    EEPROM.write(0x12, godwitCalData.accel_offset_z);
    EEPROM.write(0x13, godwitCalData.gyro_offset_x);
    EEPROM.write(0x14, godwitCalData.gyro_offset_y);
    EEPROM.write(0x15, godwitCalData.gyro_offset_z);
    EEPROM.write(0x16, godwitCalData.accel_radius);

    Serial.println("logged calibration data");
    return true;
  }
  else {
    return false;
  }
}

