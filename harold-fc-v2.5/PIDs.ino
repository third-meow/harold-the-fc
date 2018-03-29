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
  stabGains.pitch[0] = 0.001;
  stabGains.pitch[1] = 0.0002;
  stabGains.pitch[2] = 0.0006;

  stabGains.roll[0] = 0.001;
  stabGains.roll[1] = 0.0002;
  stabGains.roll[2] = 0.0006;

  manualYawGain = 0.4;

  rateGains.pitch[0] = 0.035;
  rateGains.pitch[1] = 0.01;
  rateGains.pitch[2] = 0.0035;

  rateGains.roll[0] = 0.035;
  rateGains.roll[1] = 0.01;
  rateGains.roll[2] = 0.0035;

  rateGains.yaw[0] = 0.02;
  rateGains.yaw[1] = 0.005;
  rateGains.yaw[2] = 0.002; 

}
void doPIDs() {

  rateErrors.prvPitch_I = rateErrors.pitch[2];
  rateErrors.prvRoll_I = rateErrors.roll[2];
  rateErrors.prvYaw_I = rateErrors.yaw[2];

  
  rateErrors.pitch[0] = (receiverData.pitch - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[1] = rateErrors.prvPitch_I + ((receiverData.pitch - gyro.x()) * rateGains.pitch[1]);
  rateErrors.pitch[2] = ((receiverData.pitch - gyro.x()) - (prvReceiverData.pitch - prvGyro.x())) * rateGains.pitch[2];
  rateErrors.pitch[3] = rateErrors.pitch[0] + rateErrors.pitch[1] + rateErrors.pitch[2];

  rateErrors.roll[0] = (receiverData.roll - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[1] = rateErrors.prvRoll_I + ((receiverData.roll - gyro.y()) * rateGains.roll[1]);
  rateErrors.roll[2] = ((receiverData.roll - gyro.y()) - (prvReceiverData.roll - prvGyro.y())) * rateGains.roll[2];
  rateErrors.roll[3] = rateErrors.roll[0] + rateErrors.roll[1] + rateErrors.roll[2];

  rateErrors.yaw[0] = (receiverData.yaw - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[1] = rateErrors.prvYaw_I + ((receiverData.yaw - gyro.z()) * rateGains.yaw[1]);
  rateErrors.yaw[2] = ((receiverData.yaw - gyro.z()) - (prvReceiverData.yaw - prvGyro.z())) * rateGains.yaw[2];
  rateErrors.yaw[3] = rateErrors.yaw[0] + rateErrors.yaw[1] + rateErrors.yaw[2];
}

void resetI(){
  rateErrors.prvPitch_I = rateErrors.prvRoll_I = rateErrors.prvYaw_I = 0;
}

