int initReceiver() {
  ch1.pin = 12;     //set all pwm pins to their channel's pin variable
  ch2.pin = 18;
  ch3.pin = 17;
  ch4.pin = 16;
  ch5.pin = 15;
  ch6.pin = 14;

  pinMode(ch1.pin, INPUT);    //set all channel's pins to input
  pinMode(ch2.pin, INPUT);
  pinMode(ch3.pin, INPUT);
  pinMode(ch4.pin, INPUT);
  pinMode(ch5.pin, INPUT);
  pinMode(ch6.pin, INPUT);

  attachInterrupt(ch1.pin, sigChange1, CHANGE);   //attach all interputs
  attachInterrupt(ch2.pin, sigChange2, CHANGE);
  attachInterrupt(ch3.pin, sigChange3, CHANGE);
  attachInterrupt(ch4.pin, sigChange4, CHANGE);
  attachInterrupt(ch5.pin, sigChange5, CHANGE);
  attachInterrupt(ch6.pin, sigChange6, CHANGE);

  //if there is no good signal immediately after reset harold will 
  //default to .prvPulseWidth so they must be set
  ch1.prvPulseWidth = RC_CH1_MIN; //channels 1-4 are set to their min
  ch2.prvPulseWidth = RC_CH2_MIN;
  ch3.prvPulseWidth = RC_CH3_MIN;
  ch4.prvPulseWidth = RC_CH4_MIN;
  ch5.prvPulseWidth = RC_CH5_MIN; //channel 5 must be set low because it is the arming switch
  ch6.prvPulseWidth = (RC_CH6_MAX - RC_CH6_MIN); //channel 6 set to center becuase thats normal flight mode

  ch1.pulse = digitalRead(ch1.pin);     //set boolean pulse variable to current state of the channels pin
  ch2.pulse = digitalRead(ch2.pin);
  ch3.pulse = digitalRead(ch3.pin);
  ch4.pulse = digitalRead(ch4.pin);
  ch5.pulse = digitalRead(ch5.pin);
  ch6.pulse = digitalRead(ch6.pin);

  return 1;
}

void updateReceiver(uint16_t *throttle, int16_t *yaw, int8_t *pitch, int8_t *roll, uint8_t *mode, bool *arm) {

  prvReceiverData = receiverData;
  
  if (ch1.pulseWidth > (RC_CH1_MAX + 15) || ch1.pulseWidth < (RC_CH1_MIN - 15)) {
    ch1.pulseWidth = ch1.prvPulseWidth;
  }

  if (ch2.pulseWidth > (RC_CH2_MAX + 15) || ch2.pulseWidth < (RC_CH2_MIN - 15)) {
    ch2.pulseWidth = ch2.prvPulseWidth;
  }

  if (ch3.pulseWidth > (RC_CH3_MAX + 15) || ch3.pulseWidth < (RC_CH3_MIN - 15)) {
    ch3.pulseWidth = ch3.prvPulseWidth;
  }

  if (ch4.pulseWidth > (RC_CH4_MAX + 15) || ch4.pulseWidth < (RC_CH4_MIN - 15)) {
    ch4.pulseWidth = ch4.prvPulseWidth;
  }

  if (ch5.pulseWidth > (RC_CH5_MAX + 15) || ch5.pulseWidth < (RC_CH5_MIN - 15)) {
    ch5.pulseWidth = ch5.prvPulseWidth;
  }

  if (ch6.pulseWidth > (RC_CH6_MAX + 15) || ch6.pulseWidth < (RC_CH6_MIN - 15)) {
    ch6.pulseWidth = ch6.prvPulseWidth;
  }

  *roll = map(ch1.pulseWidth, RC_CH1_MIN, RC_CH1_MAX, -MAXROLL, MAXROLL);
  if (abs(*roll) < 5) *roll = 0;
  
  *pitch = map(ch2.pulseWidth, RC_CH2_MIN, RC_CH2_MAX, -MAXPITCH, MAXPITCH);
  if (abs(*pitch) < 5) *pitch = 0;
  
  *throttle = ch3.pulseWidth;
  
  *yaw = map(ch4.pulseWidth, RC_CH4_MIN, RC_CH4_MAX, -180, 180);
  if (abs(*yaw) < 5) *yaw = 0;
  
  *mode = constrain(map(ch6.pulseWidth, RC_CH6_MIN, RC_CH6_MAX, 1, 3), 1, 3);   //map and constrain the pulse width of ch6 to 1-3 and set to *mode
  
  if (ch5.pulseWidth > 1500) {      //if the pulse width of ch5 is higher then 1500 (center) *arm is true
    *arm = true;
  } else {          //else *arm is false
    *arm = false;
  }

  ch1.prvPulseWidth = ch1.pulseWidth;    //set previous pulsewidths
  ch2.prvPulseWidth = ch2.pulseWidth;
  ch3.prvPulseWidth = ch3.pulseWidth;
  ch4.prvPulseWidth = ch4.pulseWidth;
  ch5.prvPulseWidth = ch5.pulseWidth;
  ch6.prvPulseWidth = ch6.pulseWidth;
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
