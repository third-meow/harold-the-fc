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
  stabGains.pitch[1] = 0.00008;
  stabGains.pitch[2] = 1;
  stabGains.roll[0] = 10;
  stabGains.roll[1] = 0.00008;
  stabGains.roll[2] = 1;
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
  //error = desired - actual
  stabErrors.pitchError = receiverData.pitch - euler.z();
  
  //P = error * gain
  stabErrors.pitch[0] = stabErrors.pitchError * stabGains.pitch[0];
  
  //I = previous I + error * gain
  stabErrors.pitch[1] = stabErrors.pitch[1] + stabErrors.pitchError * stabGains.pitch[1];
  
  //D = error - previous error * gain
  stabErrors.pitch[2] = (stabErrors.pitchError - stabErrors.prvPitchError) * stabGains.pitch[2];
  
  //output = P + I + D
  stabErrors.pitch[3] = stabErrors.pitch[0] + stabErrors.pitch[1] + stabErrors.pitch[2];
  
  //previous error = error
  stabErrors.prvPitchError = stabErrors.pitchError;
  
  /*
   **************************************************************************************************
   */
  
  //error = desired - actual
  stabErrors.rollError = receiverData.roll - euler.y();

  //P = error * gain
  stabErrors.roll[0] = stabErrors.rollError * stabGains.roll[0];
  
  //I = previous I + error * gain
  stabErrors.roll[1] = stabErrors.roll[1] + stabErrors.rollError * stabGains.roll[1];
  
  //D = error - previous error * gain
  stabErrors.roll[2] = (stabErrors.rollError - stabErrors.prvRollError) * stabGains.roll[2];
  
  //output = P + I + D
  stabErrors.roll[3] = stabErrors.roll[0] + stabErrors.roll[1] + stabErrors.roll[2];
  
  //previous error = error
  stabErrors.prvRollError = stabErrors.rollError;  
  
  /*
   **************************************************************************************************
   */
  
  //P = error * gain
  rateErrors.pitch[0] = (stabErrors.pitch[3] - gyro.x()) * rateGains.pitch[0];
  //I = previous I + error * gain
  rateErrors.pitch[1] = rateErrors.pitch[1] + (stabErrors.pitch[3] - gyro.x()) * rateGains.pitch[1];
  //output = P + I
  rateErrors.pitch[3] = rateErrors.pitch[0] + rateErrors.pitch[1];
  
  //P = error * gain
  rateErrors.roll[0] = (stabErrors.roll[3] - gyro.y()) * rateGains.roll[0];
  //I = previous I + error * gain
  rateErrors.roll[1] = rateErrors.roll[1] + (stabErrors.roll[3] - gyro.y()) * rateGains.roll[1];
  //output = P + I
  rateErrors.roll[3] = rateErrors.roll[0] + rateErrors.roll[1];
  
  //P = error * gain
  rateErrors.yaw[0] = (receiverData.yaw - gyro.z()) * rateGains.yaw[0];
  //I = previous I + error * gain
  rateErrors.yaw[1] = rateErrors.yaw[1] + (receiverData.yaw - gyro.z()) * rateGains.yaw[1];
  //output = P + I
  rateErrors.yaw[3] = constrain(rateErrors.yaw[0] + rateErrors.yaw[1], -rateGains.yawMax[3], rateGains.yawMax[3]);
}

void resetI(){
  rateErrors.pitch[1] = rateErrors.roll[1] = rateErrors.yaw[1] = 0;
}

