int initIMU() {
  if (godwit.begin())
  {
    delay(200);
    godwit.setExtCrystalUse(true);
    sensors_event_t IMUData;
    return 1;
  }
  else {
    return -1;
  }
}


void updateIMU() {
  godwit.getEvent(&IMUData);
}
