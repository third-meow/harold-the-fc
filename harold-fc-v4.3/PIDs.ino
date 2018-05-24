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
  rateGains.pitch[0] = 0.007;
  rateGains.pitch[1] = 0.0000056;
  rateGains.roll[0] = 0.007;
  rateGains.roll[1] = 0.0000056;
  rateGains.yaw[0] = 0.007;
  rateGains.yaw[1] = 0.0000056;
  rateGains.yawMax[3] = 30;
}

void setPrvI(){
  rateErrors.prvPitch_I = rateErrors.pitch[1];
  rateErrors.prvRoll_I = rateErrors.roll[1];
  rateErrors.prvYaw_I = rateErrors.yaw[1]; 
}

void doPIDs() {
  rateErrors.pitch[0] = (receiverData.pitch - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[1] = rateErrors.pitch[1] + (receiverData.pitch - gyro.x()) * rateGains.pitch[1];
  rateErrors.pitch[3] = rateErrors.pitch[0] + rateErrors.pitch[1];
  
  rateErrors.roll[0] = (receiverData.roll - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[1] = rateErrors.roll[1] + (receiverData.roll - gyro.y()) * rateGains.roll[1];
  rateErrors.roll[3] = rateErrors.roll[0] + rateErrors.roll[1];
  
  rateErrors.yaw[0] = (receiverData.yaw - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[1] = rateErrors.yaw[1] + (receiverData.yaw - gyro.z()) * rateGains.yaw[1];
  rateErrors.yaw[3] = constrain(rateErrors.yaw[0] + rateErrors.yaw[1], -rateGains.yawMax[3], rateGains.yawMax[3]);
}

void resetI(){
  rateErrors.pitch[1] = rateErrors.roll[1] = rateErrors.yaw[1] = 0;
}

