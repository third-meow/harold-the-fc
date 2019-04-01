Servo back;
Servo front;
Servo left;
Servo right;
void setFrontMotor(int val) {
  front.writeMicroseconds(val + 1000);
}

void setBackMotor(int val) {
  back.writeMicroseconds(val + 1000);
}

void setLeftMotor(int val) {
  left.writeMicroseconds(val + 1000);
}

void setRightMotor(int val) {
  right.writeMicroseconds(val + 1000);
}

void initMotors() {
  digitalWrite(LED_BUILTIN, LOW);
  back.attach(PB1);
  front.attach(PA10);
  left.attach(PA9);
  right.attach(PA8);
  delay(2600);


  back.writeMicroseconds(1000);
  front.writeMicroseconds(1000);
  left.writeMicroseconds(1000);
  right.writeMicroseconds(1000);
  delay(2600);

  for (int i = 1000; i < 1500; i += 10) {
    back.writeMicroseconds(i);
    front.writeMicroseconds(i);
    left.writeMicroseconds(i);
    right.writeMicroseconds(i);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2600);

  for (int i = 1490; i > 1000; i -= 10) {
    back.writeMicroseconds(i);
    front.writeMicroseconds(i);
    left.writeMicroseconds(i);
    right.writeMicroseconds(i);
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(2600);

  setBackMotor(100);
  setFrontMotor(100);
  setLeftMotor(100);
  setRightMotor(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  setBackMotor(0);
  setFrontMotor(0);
  setLeftMotor(0);
  setRightMotor(0);
  digitalWrite(LED_BUILTIN, LOW);
}
