#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

imu::Vector<3> comp;

//setup BNO055
Adafruit_BNO055 godwit = Adafruit_BNO055(19, 0x29);

void setup() {
  initIMU();
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(9600);
  delay(1000);

}

void loop() {
  digitalWrite(LED_BUILTIN,HIGH);
  delay(28);
  digitalWrite(LED_BUILTIN,LOW);
  delay(27);
  comp = godwit.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  Serial.println((atan2(comp.y(),comp.x()) * 180) / PI);
}

int initIMU() {
  if (godwit.begin())
  {
    delay(200);
    godwit.setExtCrystalUse(true);
    return 1;       
  }
  else {
    return -1;
  }
}

