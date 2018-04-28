int wrapOneEighty(int input) {
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
  rateGains.pitch[0] = 0.5;
  rateGains.roll[0] = 0.5;
  rateGains.yaw[0] = 0.5;
}

void setPrvI(){
  rateErrors.prvPitch_I = rateErrors.pitch[1];
  rateErrors.prvRoll_I = rateErrors.roll[1];
  rateErrors.prvYaw_I = rateErrors.yaw[1]; 
}

void doPIDs() {
  rateErrors.pitch[0] = (receiverData.pitch - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[3] = rateErrors.pitch[0];
  
  rateErrors.roll[0] = (receiverData.roll - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[3] = rateErrors.roll[0];
  
  rateErrors.yaw[0] = (receiverData.yaw - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[3] = rateErrors.yaw[0];
}

void resetI(){
  rateErrors.prvPitch_I = rateErrors.prvRoll_I = rateErrors.prvYaw_I = 0;
}

