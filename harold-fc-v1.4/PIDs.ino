int wrapOneEighty(int input) {
  if (input < -180) {
    input += 360;
  } else if (input > 180) {
    input -= 360;
  }
  return input;
}
void resetYawTarget(){
  yawTarget = euler.x();
}
void setGains() {
  stabGains.pitch[0] = 0.0000001;
  stabGains.pitch[1] = 0.00000002;
  stabGains.pitch[2] = 0.00000006;

  stabGains.roll[0] = 0.0000001;
  stabGains.roll[1] = 0.00000002;
  stabGains.roll[2] = 0.00000006;

  manualYawGain = 0.4;

  rateGains.pitch[0] = 0.0000001;
  rateGains.pitch[1] = 0.00000002;
  rateGains.pitch[2] = 0.00000006;

  rateGains.roll[0] = 0.0000001;
  rateGains.roll[1] = 0.00000002;
  rateGains.roll[2] = 0.00000006;

  rateGains.yaw[0] = 0.0000001;
  rateGains.yaw[1] = 0.00000002;
  rateGains.yaw[2] = 0.00000006;
  
}
void doPIDs() {

  prvStabErrors = stabErrors;

  stabErrors.pitch[0] = (receiverData.pitch - euler.z()) * stabGains.pitch[0];
  stabErrors.pitch[1] = stabErrors.prvPitch_I + ((receiverData.pitch - euler.z()) * stabGains.pitch[1]);
  stabErrors.pitch[2] = ((receiverData.pitch - euler.z()) - (prvReceiverData.pitch - prvEuler.z())) * stabGains.pitch[2];
  stabErrors.pitch[3] = stabErrors.pitch[0] + stabErrors.pitch[1] + stabErrors.pitch[2];

  stabErrors.roll[0] = (receiverData.roll - euler.y()) * stabGains.roll[0];
  stabErrors.roll[1] = stabErrors.prvRoll_I + ((receiverData.roll - euler.y()) * stabGains.roll[1]);
  stabErrors.roll[2] = ((receiverData.roll - euler.y()) - (prvReceiverData.roll - prvEuler.y())) * stabGains.roll[2];
  stabErrors.roll[3] = stabErrors.roll[0] + stabErrors.roll[1] + stabErrors.roll[2];

  //if there's no yaw input (more than 5 - see deadband calc) run stabilisation PID based on latest yawTarget from reciver
  if (!receiverData.yaw) {
    stabErrors.yaw[0] = (yawTarget - euler.x()) * stabGains.yaw[0];
    stabErrors.yaw[1] = stabErrors.prvYaw_I + ((yawTarget - euler.x()) * stabGains.yaw[1]);
    stabErrors.yaw[2] = ((yawTarget - euler.x()) - (yawTarget - prvEuler.x())) * stabGains.yaw[2];  //note no prvYawTarget used because yawTarget will be the same unless user changes it
    stabErrors.yaw[3] = stabErrors.yaw[0] + stabErrors.yaw[1] + stabErrors.yaw[2];
  } else {  //else send input stright to stabErrors
    stabErrors.yaw[3] = receiverData.yaw * manualYawGain;
    resetYawTarget();
  }


  rateErrors.pitch[0] = (stabErrors.pitch[3] - gyro.x()) * rateGains.pitch[0];
  rateErrors.pitch[1] = rateErrors.prvPitch_I + ((stabErrors.pitch[3] - gyro.x()) * rateGains.pitch[1]);
  rateErrors.pitch[2] = ((stabErrors.pitch[3] - gyro.x()) - (prvStabErrors.pitch[3] - prvGyro.x())) * rateGains.pitch[2];
  rateErrors.pitch[3] = rateErrors.pitch[0] + rateErrors.pitch[1] + rateErrors.pitch[2];

  rateErrors.roll[0] = (stabErrors.roll[3] - gyro.y()) * rateGains.roll[0];
  rateErrors.roll[1] = rateErrors.prvRoll_I + ((stabErrors.roll[3] - gyro.y()) * rateGains.roll[1]);
  rateErrors.roll[2] = ((stabErrors.roll[3] - gyro.y()) - (prvStabErrors.roll[3] - prvGyro.y())) * rateGains.roll[2];
  rateErrors.roll[3] = rateErrors.roll[0] + rateErrors.roll[1] + rateErrors.roll[2];

  rateErrors.yaw[0] = (stabErrors.yaw[3] - gyro.z()) * rateGains.yaw[0];
  rateErrors.yaw[1] = rateErrors.prvYaw_I + ((stabErrors.yaw[3] - gyro.z()) * rateGains.yaw[1]);
  rateErrors.yaw[2] = ((stabErrors.yaw[3] - gyro.z()) - (prvStabErrors.yaw[3] - prvGyro.z())) * rateGains.yaw[2];
  rateErrors.yaw[3] = rateErrors.yaw[0] + rateErrors.yaw[1] + rateErrors.yaw[2];
}

