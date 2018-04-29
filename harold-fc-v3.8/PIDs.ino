/*int16_t wrapOneEighty(int16_t input) {
  if (input < -180) {
    input += 360;
  } else if (input > 180) {
    input -= 360;
  }
  return input;
}*/

void setGains() {
  rateGains.pitch[0] = 0.012;
  rateGains.pitch[1] = 0.00003;
  rateGains.pitchMax[1] = 60;
  rateGains.pitchMax[3] = 200;
  
  rateGains.roll[0] = 0.012;
  rateGains.roll[1] = 0.00003;
  rateGains.rollMax[1] = 60;
  rateGains.rollMax[3] = 200;
  
  rateGains.yaw[0] = 0;
  rateGains.yaw[1] = 0;
  rateGains.yawMax[1] = 30;
  rateGains.yawMax[3] = 150;
}

void doPIDs() {
  rateErrors.pitch[0] = (receiverData.pitch - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[1] = constrain(rateErrors.pitch[1] + (receiverData.pitch - gyro.x()) * rateGains.pitch[1], -rateGains.pitchMax[1], rateGains.pitchMax[1]);
  rateErrors.pitch[3] = constrain(rateErrors.pitch[0] + rateErrors.pitch[1], -rateGains.pitchMax[3], rateGains.pitchMax[3]);
  
  rateErrors.roll[0] = (receiverData.roll - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[1] = constrain(rateErrors.roll[1] + (receiverData.roll - gyro.y()) * rateGains.roll[1], -rateGains.rollMax[1], rateGains.rollMax[1]);
  rateErrors.roll[3] = constrain(rateErrors.roll[0] + rateErrors.roll[1], -rateGains.rollMax[3], rateGains.rollMax[3]);
  
  rateErrors.yaw[0] = (receiverData.yaw - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[1] = constrain(rateErrors.yaw[1] + (receiverData.yaw - gyro.z()) * rateGains.yaw[1], -rateGains.yawMax[1], rateGains.yawMax[1]);
  rateErrors.yaw[3] = constrain(rateErrors.yaw[0], -rateGains.yawMax[3], rateGains.yawMax[3]);
}

void resetI(){
  rateErrors.pitch[1] = rateErrors.roll[1] = rateErrors.yaw[1] = 0;
}

