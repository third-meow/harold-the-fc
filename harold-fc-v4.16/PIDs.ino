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
  stabGains.pitch[1] = 0.000006;
  stabGains.pitch[2] = 7;
  stabGains.roll[0] = 10;
  stabGains.roll[1] = 0.000006;
  stabGains.roll[2] = 7;
  
  rateGains.pitch[0] = 0.007;
  rateGains.pitch[1] = 0.0000056;
  rateGains.roll[0] = 0.007;
  rateGains.roll[1] = 0.0000056;
  rateGains.yaw[0] = 0.013;
  rateGains.yaw[1] = 0.0000112;
  rateGains.yawMax[3] = 50;
}

void doPIDs() {
  stabErrors.pitchError = receiverData.pitch - euler.z();                                        //calculate plain error
  stabErrors.pitch[0] = stabErrors.pitchError * stabGains.pitch[0];                              //calculate P
  stabErrors.pitch[1] = stabErrors.pitch[1] + stabErrors.pitchError * stabGains.pitch[1];        //calculate I
  stabErrors.pitch[2] = (stabErrors.pitchError - stabErrors.prvPitchError) * stabGains.pitch[2]; //calculate D        
  stabErrors.pitch[3] = stabErrors.pitch[0] + stabErrors.pitch[1];// + stabErrors.pitch[2];         //calculate PID
  stabErrors.prvPitchError = stabErrors.pitchError;                                              //set prv plain error
  
  
  stabErrors.rollError = receiverData.roll - euler.y();
  stabErrors.roll[0] = stabErrors.rollError * stabGains.roll[0];
  stabErrors.roll[1] = stabErrors.roll[1] + stabErrors.rollError * stabGains.roll[1];
  stabErrors.roll[2] = (stabErrors.rollError - stabErrors.prvRollError) * stabGains.roll[2];
  stabErrors.roll[3] = stabErrors.roll[0] + stabErrors.roll[1];// + stabErrors.roll[2];
  stabErrors.prvRollError = stabErrors.rollError;



  rateErrors.pitchError = stabErrors.pitch[3] - gyro.x();
  rateErrors.pitch[0] = rateErrors.pitchError * rateGains.pitch[0];
  rateErrors.pitch[1] = rateErrors.pitch[1] + rateErrors.pitchError * rateGains.pitch[1];
  rateErrors.pitch[3] = rateErrors.pitch[0] + rateErrors.pitch[1];
  
  rateErrors.rollError = stabErrors.roll[3] - gyro.y();
  rateErrors.roll[0] = rateErrors.rollError * rateGains.roll[0];
  rateErrors.roll[1] = rateErrors.roll[1] + rateErrors.rollError * rateGains.roll[1];
  rateErrors.roll[3] = rateErrors.roll[0] + rateErrors.roll[1];

  rateErrors.yawError = receiverData.yaw - gyro.z();
  rateErrors.yaw[0] = rateErrors.yawError * rateGains.yaw[0];
  rateErrors.yaw[1] = rateErrors.yaw[1] + rateErrors.yawError * rateGains.yaw[1];
  rateErrors.yaw[3] = constrain(rateErrors.yaw[0] + rateErrors.yaw[1], -rateGains.yawMax[3], rateGains.yawMax[3]);
}

void resetI(){
  rateErrors.pitch[1] = rateErrors.roll[1] = rateErrors.yaw[1] = stabErrors.pitch[1] = stabErrors.roll[1] = 0;
}

