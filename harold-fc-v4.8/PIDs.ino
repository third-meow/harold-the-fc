int16_t wrapOneEighty(int16_t input) {
  if (input < -180) {
    input += 360;
  } else if (input > 180) {
    input -= 360;
  }
  return input;
}

void resetYawTarget() {
  yawTarget = euler.x();
}

void setGains() {
  stabGains.pitch[0] = 10;
  stabGains.pitch[1] = 0.8;
  stabGains.roll[0] = 10;
  stabGains.roll[1] = 0.8;
  stabGains.yaw[0] = 10;
  
  rateGains.pitch[0] = 0.007;
  rateGains.pitch[1] = 0.0000056;
  rateGains.roll[0] = 0.007;
  rateGains.roll[1] = 0.0000056;
  rateGains.yaw[0] = 0.013;
  rateGains.yaw[1] = 0.0000112;
  rateGains.yawMax[3] = 50;
}

void doPIDs() {
  stabErrors.pitch[0] = (receiverData.pitch - euler.z()) * stabGains.pitch[0];
  stabErrors.pitch[1] = stabErrors.pitch[1] + (receiverData.pitch - euler.z()) * stabGains.pitch[1];
  stabErrors.pitch[3] = stabErrors.pitch[0] + stabErrors.pitch[1];
  
  stabErrors.roll[0] = (receiverData.roll - euler.y()) * stabGains.roll[0];
  stabErrors.roll[1] = stabErrors.roll[1] + (receiverData.roll - euler.y()) * stabGains.roll[1];
  stabErrors.roll[3] = stabErrors.roll[0];
    
  
  rateErrors.pitch[0] = (stabErrors.pitch[3] - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[1] = rateErrors.pitch[1] + (stabErrors.pitch[3] - gyro.x()) * rateGains.pitch[1];
  rateErrors.pitch[3] = rateErrors.pitch[0] + rateErrors.pitch[1];
  
  rateErrors.roll[0] = (stabErrors.roll[3] - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[1] = rateErrors.roll[1] + (stabErrors.roll[3] - gyro.y()) * rateGains.roll[1];
  rateErrors.roll[3] = rateErrors.roll[0] + rateErrors.roll[1];
  
  rateErrors.yaw[0] = (receiverData.yaw - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[1] = rateErrors.yaw[1] + (receiverData.yaw - gyro.z()) * rateGains.yaw[1];
  rateErrors.yaw[3] = constrain(rateErrors.yaw[0] + rateErrors.yaw[1], -rateGains.yawMax[3], rateGains.yawMax[3]);
}

void resetI(){
  rateErrors.pitch[1] = rateErrors.roll[1] = rateErrors.yaw[1] = 0;
}

