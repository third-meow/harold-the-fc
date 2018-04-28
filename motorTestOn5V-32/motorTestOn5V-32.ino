#include <Servo.h>

#define RC_CH3_MIN 997
#define RC_CH3_MAX 1995

Servo frontLeftMotor;
Servo frontRightMotor;
Servo backLeftMotor;
Servo backRightMotor;

void setup() {
  delay(5000);
  initMotors();
  delay(2000);
  
  for (int i = RC_CH3_MIN + 50; i < RC_CH3_MAX; i++) {
    sendMotors(i, i, i, i);
  }
  delay(500);

  
  frontLeftMotor.detach();
  frontRightMotor.detach();
  backLeftMotor.detach();
  backRightMotor.detach();
  pinMode(PC13,OUTPUT);
}

void loop() {
  digitalWrite(PC13, HIGH);
  delay(500);
  digitalWrite(PC13, LOW);
  delay(500);
}
int initMotors() {
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

  return 1;
}

void sendMotors(int fl, int fr, int bl, int br) {
  frontLeftMotor.writeMicroseconds(fl);
  frontRightMotor.writeMicroseconds(fr);
  backLeftMotor.writeMicroseconds(bl);
  backRightMotor.writeMicroseconds(br);
}

