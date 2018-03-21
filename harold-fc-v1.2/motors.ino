int initMotors() {
  //attach all motors to their respective pins
  frontLeftMotor.attach(11);
  frontRightMotor.attach(10);
  backLeftMotor.attach(5);
  backRightMotor.attach(9);
  delay(2600);

  //set all motors low
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN);
  frontRightMotor.writeMicroseconds(RC_CH3_MIN);
  backLeftMotor.writeMicroseconds(RC_CH3_MIN);
  backRightMotor.writeMicroseconds(RC_CH3_MIN);
  delay(2600);

  //bring all motors to midstick incrementally
  for (int i = RC_CH3_MIN; i < 1500; i += 10) {
    frontLeftMotor.writeMicroseconds(i);
    frontRightMotor.writeMicroseconds(i);
    backLeftMotor.writeMicroseconds(i);
    backRightMotor.writeMicroseconds(i);
  }
  delay(2600);

  //bring all motors to low incrementally
  for (int i = 1490; i > RC_CH3_MIN; i -= 10) {
    frontLeftMotor.writeMicroseconds(i);
    frontRightMotor.writeMicroseconds(i);
    backLeftMotor.writeMicroseconds(i);
    backRightMotor.writeMicroseconds(i);
  }
  delay(2600);

  frontLeftMotor.writeMicroseconds(RC_CH3_MIN + 100);
  frontRightMotor.writeMicroseconds(RC_CH3_MIN + 100);
  backLeftMotor.writeMicroseconds(RC_CH3_MIN + 100);
  backRightMotor.writeMicroseconds(RC_CH3_MIN + 100);

  return 1;
}

void sendMotors() {
  frontLeftMotor.writeMicroseconds(receiverData.throttle + rateErrors.roll[3] + rateErrors.pitch[3] - rateErrors.yaw[3]);
  Serial.print(receiverData.throttle + rateErrors.roll[3] + rateErrors.pitch[3] - rateErrors.yaw[3]);
  Serial.print("\t");
  frontRightMotor.writeMicroseconds(receiverData.throttle - rateErrors.roll[3] + rateErrors.pitch[3] + rateErrors.yaw[3]);
  Serial.print(receiverData.throttle - rateErrors.roll[3] + rateErrors.pitch[3] + rateErrors.yaw[3]);
  Serial.print("\t");
  backLeftMotor.writeMicroseconds(receiverData.throttle + rateErrors.roll[3] - rateErrors.pitch[3] + rateErrors.yaw[3]);
  Serial.print(receiverData.throttle + rateErrors.roll[3] - rateErrors.pitch[3] + rateErrors.yaw[3]);
  Serial.print("\t");
  backRightMotor.writeMicroseconds(receiverData.throttle - rateErrors.roll[3] - rateErrors.pitch[3] - rateErrors.yaw[3]);
  Serial.print(receiverData.throttle - rateErrors.roll[3] - rateErrors.pitch[3] - rateErrors.yaw[3]);
  Serial.print("\n");
}

void stopMotors() {
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN + 50);
  frontRightMotor.writeMicroseconds(RC_CH3_MIN + 50);
  backLeftMotor.writeMicroseconds(RC_CH3_MIN + 50);
  backRightMotor.writeMicroseconds(RC_CH3_MIN + 50);
}

