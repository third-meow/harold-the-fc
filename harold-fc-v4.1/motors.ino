bool initMotors() {
  //attach all motors to their respective pins
  frontLeftMotor.attach(PA10);
  frontRightMotor.attach(PA9);
  backLeftMotor.attach(PB1);
  backRightMotor.attach(PA8);
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

  return true;
}

void sendMotors() {
  frontLeftMotor.writeMicroseconds(receiverData.throttle + rateErrors.roll[3] + rateErrors.pitch[3] - rateErrors.yaw[3]);
  frontRightMotor.writeMicroseconds(receiverData.throttle - rateErrors.roll[3] + rateErrors.pitch[3] + rateErrors.yaw[3]);
  backLeftMotor.writeMicroseconds(receiverData.throttle + rateErrors.roll[3] - rateErrors.pitch[3] + rateErrors.yaw[3]);
  backRightMotor.writeMicroseconds(receiverData.throttle - rateErrors.roll[3] - rateErrors.pitch[3] - rateErrors.yaw[3]);

  /*Serial.println(receiverData.throttle + rateErrors.roll[3] + rateErrors.pitch[3] - rateErrors.yaw[3]);
  Serial.print("\t"); Serial.print(receiverData.throttle - rateErrors.roll[3] + rateErrors.pitch[3] + rateErrors.yaw[3]);
  Serial.print("\t"); Serial.print(receiverData.throttle + rateErrors.roll[3] - rateErrors.pitch[3] + rateErrors.yaw[3]);
  Serial.print("\t"); Serial.println(receiverData.throttle - rateErrors.roll[3] - rateErrors.pitch[3] - rateErrors.yaw[3]);*/

}

void stopMotors() {
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN + 5);
  frontRightMotor.writeMicroseconds(RC_CH3_MIN + 5);
  backLeftMotor.writeMicroseconds(RC_CH3_MIN + 5);
  backRightMotor.writeMicroseconds(RC_CH3_MIN + 5);
}

