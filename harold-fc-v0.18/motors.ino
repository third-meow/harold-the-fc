void initMotors() {
  frontLeftMotor.attach(11);
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN);

  frontRightMotor.attach(10);
  frontRightMotor.writeMicroseconds(RC_CH3_MIN);

  backLeftMotor.attach(6);
  backLeftMotor.writeMicroseconds(RC_CH3_MIN);

  backRightMotor.attach(9);
  backRightMotor.writeMicroseconds(RC_CH3_MIN);
}

void sendMotors() {
  frontLeftMotor.writeMicroseconds(receiverData.throttle + rateErrors.roll[3] + rateErrors.pitch[3] - rateErrors.yaw[3]);
  frontRightMotor.writeMicroseconds(receiverData.throttle - rateErrors.roll[3] + rateErrors.pitch[3] + rateErrors.yaw[3]);
  backLeftMotor.writeMicroseconds(receiverData.throttle + rateErrors.roll[3] - rateErrors.pitch[3] + rateErrors.yaw[3]);
  backRightMotor.writeMicroseconds(receiverData.throttle - rateErrors.roll[3] - rateErrors.pitch[3] - rateErrors.yaw[3]);
}

void stopMotors(){
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN);
  frontRightMotor.writeMicroseconds(RC_CH3_MIN);
  backLeftMotor.writeMicroseconds(RC_CH3_MIN);
  backRightMotor.writeMicroseconds(RC_CH3_MIN);
}

