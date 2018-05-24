bool initIMU() {
  if (godwit.begin()) {
    delay(200);
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






    godwit.setExtCrystalUse(true);
    return true;
  }
  else {
    return false;
  }
}

int getIMUCalStatus() {

  uint8_t sys, gy, acl, mg;
  sys = gy = acl = mg = -1;
  godwit.getCalibration(&sys, &gy, &acl, &mg);

  if (SERIAL_OUT == true) {
    Serial.print("SYSTEM : ");
    Serial.println(sys);

    Serial.print("GYRO : ");
    Serial.println(gy);

    Serial.print("MAG : ");
    Serial.println(mg);
    Serial.println("\n");
  }
  if (sys == 3 && gy == 3 && mg == 3) {
    return true;
  }
}
