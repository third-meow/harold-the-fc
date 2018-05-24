void setGains() {
  rateGains.pitch[0] = 0.025;
  rateGains.pitch[1] = 0.00002;
  rateGains.pitchMax[1] = 80;
  rateGains.pitchMax[3] = 300;
  
  rateGains.roll[0] = 0.025;
  rateGains.roll[1] = 0.00002;
  rateGains.rollMax[1] = 80;
  rateGains.rollMax[3] = 300;
  
  rateGains.yaw[0] = 0.025;
  rateGains.yaw[1] = 0.00002;
  rateGains.yawMax[1] = 80;
  rateGains.yawMax[3] = 300;
}

void doPIDs() {
  rateErrors.pitch[0] = (0 - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[1] = rateErrors.pitch[1] + (0 - gyro.x()) * rateGains.pitch[1];
  //rateErrors.pitch[3] = rateErrors.pitch[0];
  
  rateErrors.roll[0] = (0 - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[1] = rateErrors.roll[1] + (0 - gyro.y()) * rateGains.roll[1];
  rateErrors.roll[3] = rateErrors.roll[0];
  
  rateErrors.yaw[0] = (0 - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[1] = rateErrors.yaw[1] + (0 - gyro.z()) * rateGains.yaw[1];
  //rateErrors.yaw[3] = rateErrors.yaw[0], -rateGains.yawMax[3], rateGains.yawMax[3]);
}

void resetI(){
  rateErrors.pitch[1] = rateErrors.roll[1] = rateErrors.yaw[1] = 0;
}

