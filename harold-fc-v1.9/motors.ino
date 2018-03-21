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
  frontLeftMotor.writeMicroseconds(constrain(receiverData.throttle + rateErrors.roll[0] + rateErrors.pitch[0] - rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print(constrain(receiverData.throttle + rateErrors.roll[0] + rateErrors.pitch[0] - rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print("\t");
  frontRightMotor.writeMicroseconds(constrain(receiverData.throttle - rateErrors.roll[0] + rateErrors.pitch[0] + rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print(constrain(receiverData.throttle - rateErrors.roll[0] + rateErrors.pitch[0] + rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print("\t");
  backLeftMotor.writeMicroseconds(constrain(receiverData.throttle + rateErrors.roll[0] - rateErrors.pitch[0] + rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print(constrain(receiverData.throttle + rateErrors.roll[0] - rateErrors.pitch[0] + rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print("\t");
  backRightMotor.writeMicroseconds(constrain(receiverData.throttle - rateErrors.roll[0] - rateErrors.pitch[0] - rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print(constrain(receiverData.throttle - rateErrors.roll[0] - rateErrors.pitch[0] - rateErrors.yaw[0], RC_CH3_MIN, RC_CH3_MAX));
  //Serial.print("\n");
}

void stopMotors() {
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN + 5);
  frontRightMotor.writeMicroseconds(RC_CH3_MIN + 5);
  backLeftMotor.writeMicroseconds(RC_CH3_MIN + 5);
  backRightMotor.writeMicroseconds(RC_CH3_MIN + 5);
}

