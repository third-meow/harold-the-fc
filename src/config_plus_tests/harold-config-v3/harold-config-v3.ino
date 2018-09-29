#include <Servo.h>

#define RC_CH3_MIN 997
#define RC_CH3_MAX 1995

#define LED_INSIDE PC13

Servo frontLeftMotor;
Servo frontRightMotor;
Servo backLeftMotor;
Servo backRightMotor;

void setup() {
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
  delay(600);

  //bring all motors to high incrementally
  for (int i = RC_CH3_MIN; i < RC_CH3_MAX; i += 10) {
    frontLeftMotor.writeMicroseconds(i);
    frontRightMotor.writeMicroseconds(i);
    backLeftMotor.writeMicroseconds(i);
    backRightMotor.writeMicroseconds(i);
  }
  delay(5000);

  //bring all motors to low incrementally
  for (int i = RC_CH3_MAX; i > RC_CH3_MIN; i -= 10) {
    frontLeftMotor.writeMicroseconds(i);
    frontRightMotor.writeMicroseconds(i);
    backLeftMotor.writeMicroseconds(i);
    backRightMotor.writeMicroseconds(i);
  }
  delay(5000);  
}

void loop() {
}
