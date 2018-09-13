bool initIMU() {
  if (godwit.begin()) {
    delay(200);
    godwit.setExtCrystalUse(true);
    updateIMU();
    updateIMU();
    return true;
  }
  else {
    return false;
  }
}


void updateIMU() {
  prvGyro = gyro;
  prvEuler = euler;
  gyro = godwit.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE) * 180 / PI;
  euler = godwit.getVector(Adafruit_BNO055::VECTOR_EULER);

  if (gyro.z() < 20 && gyro.z() > -20){
    gyro.z() = 0;
  }

  
}

int getIMUCalStatus() {
  uint8_t sys, gy, acl, mg;
  sys = gy = acl = mg = -1;
  godwit.getCalibration(&sys, &gy, &acl, &mg);

  Serial1.print("SYSTEM : ");
  Serial1.println(sys);

  Serial1.print("GYRO : ");
  Serial1.println(gy);

  Serial1.print("MAG : ");
  Serial1.println(mg);
  Serial1.println("\n");


  if (sys == 3 && gy == 3 && mg == 3) {
    return true;
  }
}


