int initIMU() {
  if (godwit.begin())
  {
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
}
