#include <Servo.h>

#define RC_CH3_MIN 997
#define RC_CH3_MAX 1995

Servo frontLeftMotor;
Servo frontRightMotor;
Servo backLeftMotor;
Servo backRightMotor;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("about to start motors");
  initMotors();
  Serial.println("waiting");
  delay(2000);
  Serial.println("done waiting");
  
  
  for (int i = RC_CH3_MIN + 50; i < RC_CH3_MAX; i++) {
    sendMotors(i, i, i, i);
  }
  Serial.println("set high");
  delay(500);

  
  frontLeftMotor.detach();
  frontRightMotor.detach();
  backLeftMotor.detach();
  backRightMotor.detach();
  Serial.println("motors detached");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
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

void sendMotors(int fl, int fr, int bl, int br) {
  frontLeftMotor.writeMicroseconds(fl);
  frontRightMotor.writeMicroseconds(fr);
  backLeftMotor.writeMicroseconds(bl);
  backRightMotor.writeMicroseconds(br);
}

