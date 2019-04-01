Servo back;
Servo front;
Servo left;
Servo right;

void setAllMotors(int val) {
  setBackMotor(val);
  setFrontMotor(val);
  setLeftMotor(val);
  setRightMotor(val);
}

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


	setAllMotors(1000);
  delay(2600);

  for (int i = 1000; i < 1500; i += 10) {
		setAllMotors(i);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2600);

  for (int i = 1490; i > 1000; i -= 10) {
		setAllMotors(i);
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(2600);

	setAllMotors(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
	setAllMotors(0);
  digitalWrite(LED_BUILTIN, LOW);
}

