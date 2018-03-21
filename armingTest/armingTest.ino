#include <Servo.h>

#define RC_CH3_MIN 997
#define RC_CH3_MAX 1995

Servo frontLeftMotor;


void setup() {
  Serial.begin(9600);
  while (!Serial){}
  Serial.println("about to start motor");
  initMotors();
  Serial.println("waiting");
  delay(2000);
  Serial.println("done waiting");
  for (int i = RC_CH3_MIN + 100; i < RC_CH3_MAX; i += 10) {
    frontLeftMotor.writeMicroseconds(i);
    delay(10);
  }
  Serial.println("set high");
  delay(500);
  frontLeftMotor.detach();
  Serial.println("motor detached");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

int initMotors() {
  frontLeftMotor.attach(5);
  Serial.println("motor attached");
  delay(2600);
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN);
  Serial.println("set low");
  delay(2600);
  for (int i = RC_CH3_MIN; i < 1500; i += 10) {
    frontLeftMotor.writeMicroseconds(i);
  }
  Serial.println("set mid");
  delay(2600);
  for (int i = 1490; i > RC_CH3_MIN; i -= 10) {
    frontLeftMotor.writeMicroseconds(i);
  }
  Serial.println("set low");
  delay(2600);  
  frontLeftMotor.writeMicroseconds(RC_CH3_MIN + 100);
  Serial.println("set +100");
  return 1;
}




