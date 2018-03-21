int initIMU() {
  if (godwit.begin())
  {
    delay(200);
    godwit.setExtCrystalUse(true);
    updateIMU();
    updateIMU();
    return 1;
  }
  else {
    return -1;
  }
}


void updateIMU() {
  prvGyro = gyro;
  prvEuler = euler;
  gyro = godwit.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE) * 180 / PI;
  euler = godwit.getVector(Adafruit_BNO055::VECTOR_EULER);
  Serial.print(euler.x());
  Serial.print("\t");
  Serial.print(euler.y());
  Serial.print("\t");
  Serial.print(euler.z());
  Serial.print("\n");
}
