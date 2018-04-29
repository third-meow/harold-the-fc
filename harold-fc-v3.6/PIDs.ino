/*int16_t wrapOneEighty(int16_t input) {
  if (input < -180) {
    input += 360;
  } else if (input > 180) {
    input -= 360;
  }
  return input;
}*/

void setGains() {
  rateGains.pitch[0] = 0.025;
  rateGains.pitch[1] = 0.003;
  rateGains.pitchMax = 300;
  
  rateGains.roll[0] = 0.025;
  rateGains.roll[1] = 0.003;
  rateGains.rollMax = 300;
  
  rateGains.yaw[0] = 0.025;
  rateGains.yaw[1] = 0;
  rateGains.yawMax = 300;
}

void doPIDs() {
  rateErrors.pitch[0] = (receiverData.pitch - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[1] = rateErrors.pitch[1] + (receiverData.pitch - gyro.x()) * rateGains.pitch[1];
  rateErrors.pitch[3] = constrain(rateErrors.pitch[0] + rateErrors.pitch[1], -rateGains.pitchMax, rateGains.pitchMax);
  
  rateErrors.roll[0] = (receiverData.roll - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[1] = rateErrors.roll[1] + (receiverData.roll - gyro.y()) * rateGains.roll[1];
  rateErrors.roll[3] = constrain(rateErrors.roll[0] + rateErrors.roll[1], -rateGains.rollMax, rateGains.rollMax);
  
  rateErrors.yaw[0] = (receiverData.yaw - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[1] = rateErrors.yaw[1] + (receiverData.yaw - gyro.z()) * rateGains.yaw[1];
  rateErrors.yaw[3] = constrain(rateErrors.yaw[0] + rateErrors.yaw[1], -rateGains.yawMax, rateGains.yawMax);
}

void resetI(){
  rateErrors.pitch[1] = rateErrors.roll[1] = rateErrors.yaw[1] = 0;
}

