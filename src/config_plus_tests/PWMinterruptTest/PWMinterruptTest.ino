
struct Ch {
  int pin;
  volatile bool pulse;
  volatile unsigned long endPulse;
  volatile unsigned long startPulse;
  volatile int pulseWidth;
  int lastPW;
} ch1, ch2, ch3, ch4, ch5, ch6;




void setup() {
  Serial.begin(500000);

  ch1.pin = 12;
  ch2.pin = 18;
  ch3.pin = 17;
  ch4.pin = 16;
  ch5.pin = 15;
  ch6.pin = 14;

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ch1.pin, INPUT);
  pinMode(ch2.pin, INPUT);
  pinMode(ch3.pin, INPUT);
  pinMode(ch4.pin, INPUT);
  pinMode(ch5.pin, INPUT);
  pinMode(ch6.pin, INPUT);

  attachInterrupt(ch1.pin, sigChange1, CHANGE);
  attachInterrupt(ch2.pin, sigChange2, CHANGE);
  attachInterrupt(ch3.pin, sigChange3, CHANGE);
  attachInterrupt(ch4.pin, sigChange4, CHANGE);
  attachInterrupt(ch5.pin, sigChange5, CHANGE);
  attachInterrupt(ch6.pin, sigChange6, CHANGE);

  ch1.pulse = digitalRead(ch1.pin);
  ch2.pulse = digitalRead(ch2.pin);
  ch3.pulse = digitalRead(ch3.pin);
  ch4.pulse = digitalRead(ch4.pin);
  ch5.pulse = digitalRead(ch5.pin);
  ch6.pulse = digitalRead(ch6.pin);
}

void loop() {
  delay(50);
/*
  if (ch1.pulseWidth > (ch1.lastPW + 100) || ch1.pulseWidth < (ch1.lastPW - 100)) {
    Serial.println(ch1.lastPW + "\t" + ch1.pulseWidth);
  }

  ch1.lastPW = ch1.pulseWidth;*/

  Serial.print(ch1.pulseWidth); Serial.print("\t");
    Serial.print(ch2.pulseWidth); Serial.print("\t");
    Serial.print(ch3.pulseWidth); Serial.print("\t");
    Serial.print(ch4.pulseWidth); Serial.print("\t");
    Serial.print(ch5.pulseWidth); Serial.print("\t");
    Serial.println(ch6.pulseWidth);

  if (ch6.pulseWidth > 1800) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (ch5.pulseWidth > 1800) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void sigChange1() {
  if (ch1.pulse) {
    ch1.endPulse = micros();
    ch1.pulseWidth = ch1.endPulse - ch1.startPulse;
    ch1.pulse = !ch1.pulse;
  }
  else {
    ch1.startPulse = micros();
    ch1.pulse = !ch1.pulse;
  }

}


void sigChange2() {
  if (ch2.pulse) {
    ch2.endPulse = micros();
    ch2.pulseWidth = ch2.endPulse - ch2.startPulse;
    ch2.pulse = !ch2.pulse;
  }
  else {
    ch2.startPulse = micros();
    ch2.pulse = !ch2.pulse;
  }

}


void sigChange3() {
  if (ch3.pulse) {
    ch3.endPulse = micros();
    ch3.pulseWidth = ch3.endPulse - ch3.startPulse;
    ch3.pulse = !ch3.pulse;
  }
  else {
    ch3.startPulse = micros();
    ch3.pulse = !ch3.pulse;
  }

}



void sigChange4() {
  if (ch4.pulse) {
    ch4.endPulse = micros();
    ch4.pulseWidth = ch4.endPulse - ch4.startPulse;
    ch4.pulse = !ch4.pulse;
  }
  else {
    ch4.startPulse = micros();
    ch4.pulse = !ch4.pulse;
  }

}


void sigChange5() {
  if (ch5.pulse) {
    ch5.endPulse = micros();
    ch5.pulseWidth = ch5.endPulse - ch5.startPulse;
    ch5.pulse = !ch5.pulse;
  }
  else {
    ch5.startPulse = micros();
    ch5.pulse = !ch5.pulse;
  }

}


void sigChange6() {
  if (ch6.pulse) {
    ch6.endPulse = micros();
    ch6.pulseWidth = ch6.endPulse - ch6.startPulse;
    ch6.pulse = !ch6.pulse;
  }
  else {
    ch6.startPulse = micros();
    ch6.pulse = !ch6.pulse;
  }

}


