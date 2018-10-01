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

  euler.z() -= eulerPitchOffset;
  euler.y() -= eulerRollOffset;
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
  if (sys == 3 && gy == 3 && mg == 3){
    return true;
  }
}

void setIMUlevel(){
  writeLED(true);

  updateIMU();
  updateIMU();
  updateIMU();
  eulerPitchOffset = euler.z();
  eulerRollOffset = euler.y();
  
  writeLED(false);
}
