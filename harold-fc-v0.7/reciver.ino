void initReciver() {
  ch1.pin = 12;
  ch2.pin = 18;
  ch3.pin = 17;
  ch4.pin = 16;
  ch5.pin = 15;
  ch6.pin = 14;

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

void updateReciver(int *throttle, int *yaw, int *pitch, int *roll, int *mode, bool *arm) {

  if (ch1.pulseWidth > 2000 || ch1.pulseWidth < 1000) {
    ch1.pulseWidth = ch1.lastPulseWidth;
  }

  if (ch2.pulseWidth > 2000 || ch2.pulseWidth < 1000) {
    ch2.pulseWidth = ch2.lastPulseWidth;
  }

  if (ch3.pulseWidth > 2000 || ch3.pulseWidth < 1000) {
    ch3.pulseWidth = ch3.lastPulseWidth;
  }

  if (ch4.pulseWidth > 2000 || ch4.pulseWidth < 1000) {
    ch4.pulseWidth = ch4.lastPulseWidth;
  }

  if (ch5.pulseWidth > 2000 || ch5.pulseWidth < 1000) {
    ch5.pulseWidth = ch5.lastPulseWidth;
  }

  if (ch6.pulseWidth > 2000 || ch6.pulseWidth < 1000) {
    ch6.pulseWidth = ch6.lastPulseWidth;
  }

  *throttle = constrain(map(ch3.pulseWidth, 1000, 2000, 0, 255), 0, 255);
  *yaw = constrain(map(ch4.pulseWidth, 1000, 2000, -MAXTILT, MAXTILT), -MAXTILT, MAXTILT);
  *pitch = constrain(map(ch2.pulseWidth, 1000, 2000, -MAXTILT, MAXTILT), -MAXTILT, MAXTILT);
  *roll = constrain(map(ch1.pulseWidth, 1000, 2000, -MAXTILT, MAXTILT), -MAXTILT, MAXTILT);
  *mode = constrain(map(ch6.pulseWidth, 1000, 2000, 1, 3), 1, 3);
  if (ch5.pulseWidth > 1500) {
    *arm = false;
  } else {
    *arm = true;
  }

  ch1.lastPulseWidth = ch1.pulseWidth;
  ch2.lastPulseWidth = ch2.pulseWidth;
  ch3.lastPulseWidth = ch3.pulseWidth;
  ch4.lastPulseWidth = ch4.pulseWidth;
  ch5.lastPulseWidth = ch5.pulseWidth;
  ch6.lastPulseWidth = ch6.pulseWidth;
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
