int initIMU() {
  if (godwit.begin())
  {
    delay(200);
    godwit.setExtCrystalUse(true);
    return 1;
  }
  else {
    return -1;
  }
}


void updateIMU() {
  sensors_event_t IMUData;
  godwit.getEvent(&IMUData);
}
