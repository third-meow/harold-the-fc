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


void updateIMU(int *pitch, int *roll, int *yaw, int *acelx, int *acely, int *acelz) {

}
