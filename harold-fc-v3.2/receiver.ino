bool initReceiver() {
  /*
      This code makes use of STM32F103's Input Capture Mode. This is a
      mode that timers on the STM32F103 can use. It 'captures' and stores
      the time (since last reboot) an event happens then runs an interrupt to
      do something with the infomation.

      Each timer only has 4 channels and my RC receiver has 6 channels so
      I will be using timer 2 and 3 in this program. Timer 2 takes four channels
      on pins PA0, PA1, PA2 and PA3 while timer 3 takes two channels on pins
      PA6 and PA7. If I need more channels in the future I can utilise
      timer 3's other two channels

      To use input capture mode on the STM32F103 we

      -attach interrupt handlers
      -set timers' counter enable bit (turns on in-cap-mode)
      -enable interrupts on both timers for their respective channels
      -connect input channels to edge detectors
      -set inital trigger edge in edge detector to be raising
      -set each timers' prescaler value and auto-reload vlaue
      -clear timers' other regitsors

      When an interrupt handler is run we cheak if the channel is currently high

      If so we record the value in the channel's capture compare registor
      (where the 'time' of edge detector is stored) and set the edge detector to now
      lookout for a falling edge

      If not, the channel is low so we compare the start time already recorded to current value
      found in the channel's capture compare registor to get the length of the pulse, or the pusle width
      then we set the edge detector to now lookout for a falling edge

      For more infomation on howto use input capture mode,
      see the referance manual: https://tinyurl.com/refman4bluepill and
      an amazing YouTube video by Joop Brokking : https://youtu.be/JFSFbSg0l2M

  */

  //attach interrupt handlers
  Timer2.attachCompare1Interrupt(sigChange1);
  Timer2.attachCompare2Interrupt(sigChange2);
  Timer2.attachCompare3Interrupt(sigChange3);
  Timer2.attachCompare4Interrupt(sigChange4);
  Timer3.attachCompare1Interrupt(sigChange5);
  Timer3.attachCompare2Interrupt(sigChange6);

  //set timer two and three's counter enable bit to 1
  TIMER2_BASE->CR1 = TIMER_CR1_CEN;
  TIMER3_BASE->CR1 = TIMER_CR1_CEN;

  //enable interrupts on timer two's channels 1, 2, 3 & 4.
  TIMER2_BASE->DIER = TIMER_DIER_CC1IE | TIMER_DIER_CC2IE | TIMER_DIER_CC3IE | TIMER_DIER_CC4IE;
  //enable interrupts in timer three's channels 1 and 2
  TIMER3_BASE->DIER = TIMER_DIER_CC1IE | TIMER_DIER_CC2IE;

  //connect inputs to edge detectors
  TIMER2_BASE->CCMR1 = 0b100000001;
  TIMER2_BASE->CCMR2 = 0b100000001;
  TIMER3_BASE->CCMR1 = 0b100000001;

  //enable interrupt trigger on rising edge
  TIMER2_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
  TIMER3_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E;

  //set timers' prescaler value and auto-reload value
  TIMER2_BASE->PSC = 71;
  TIMER2_BASE->ARR = 65535;
  TIMER3_BASE->PSC = 71;
  TIMER3_BASE->ARR = 65535;

  //set all timers' other registors to zero
  TIMER2_BASE->DCR = 0;
  TIMER3_BASE->DCR = 0;
  TIMER2_BASE->CR2 = 0;
  TIMER3_BASE->CR2 = 0;
  TIMER2_BASE->SMCR = 0;
  TIMER3_BASE->SMCR = 0;
  TIMER3_BASE->EGR = 0;
  TIMER2_BASE->EGR = 0;
  //only 2 channels of timer 3 are being used so timer 3 has an extra registor to set to 0
  TIMER3_BASE->CCMR2 = 0;

  return true;
}

void updateReceiver(uint16_t *throttle, int16_t *yaw, int8_t *pitch, int8_t *roll, uint8_t *mode, bool *arm) {

  prvReceiverData = receiverData;

  *roll = map(ch1.pulseWidth, RC_CH1_MIN, RC_CH1_MAX, -MAXROLL, MAXROLL);
  if (abs(*roll) < 5) *roll = 0;

  *pitch = map(ch2.pulseWidth, RC_CH2_MIN, RC_CH2_MAX, -MAXPITCH, MAXPITCH);
  if (abs(*pitch) < 5) *pitch = 0;

  *throttle = ch3.pulseWidth;

  *yaw = map(ch4.pulseWidth, RC_CH4_MIN, RC_CH4_MAX, -14, 14);
  if (abs(*yaw) < 5) *yaw = 0;

  *mode = constrain(map(ch6.pulseWidth, RC_CH6_MIN, RC_CH6_MAX, 1, 3), 1, 3);   //map and constrain the pulse width of ch6 to 1-3 and set to *mode

  if (ch5.pulseWidth > 1500) {      //if the pulse width of ch5 is higher then 1500 (center) *arm is true
    *arm = true;
  } else {          //else *arm is false
    *arm = false;
  }
}

void sigChange1() {
  //if channel is high
  if (0b1 & GPIOA_BASE->IDR  >> 0) {
    //record start time and set edge detector trigger to falling
    ch1.startPulse = TIMER2_BASE->CCR1;
    TIMER2_BASE->CCER |= TIMER_CCER_CC1P;
  }
  else {
    //calculate pulse width and set edge detector trigger to raising
    ch1.pulseWidth = TIMER2_BASE->CCR1 - ch1.startPulse;
    if (ch1.pulseWidth < 0) {
      ch1.pulseWidth += 65535;
    }
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC1P;
  }
}


void sigChange2() {
  //if channel is high
  if (0b1 & GPIOA_BASE->IDR  >> 1) {
    //record start time and set edge detector trigger to falling
    ch2.startPulse = TIMER2_BASE->CCR2;
    TIMER2_BASE->CCER |= TIMER_CCER_CC2P;
  }
  else {
    //calculate pulse width and set edge detector trigger to raising
    ch2.pulseWidth = TIMER2_BASE->CCR2 - ch2.startPulse;
    if (ch2.pulseWidth < 0) {
      ch2.pulseWidth += 65535;
    }
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC2P;
  }
}


void sigChange3() {
  //if channel is high
  if (0b1 & GPIOA_BASE->IDR  >> 2) {
    //record start time and set edge detector trigger to falling
    ch3.startPulse = TIMER2_BASE->CCR3;
    TIMER2_BASE->CCER |= TIMER_CCER_CC3P;
  }
  else {
    //calculate pulse width and set edge detector trigger to raising
    ch3.pulseWidth = TIMER2_BASE->CCR3 - ch3.startPulse;
    if (ch3.pulseWidth < 0) {
      ch3.pulseWidth += 65535;
    }
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC3P;
  }
}

void sigChange4() {
  //if channel is high
  if (0b1 & GPIOA_BASE->IDR  >> 3) {
    //record start time and set edge detector trigger to falling
    ch4.startPulse = TIMER2_BASE->CCR4;
    TIMER2_BASE->CCER |= TIMER_CCER_CC4P;
  }
  else {
    //calculate pulse width and set edge detector trigger to raising
    ch4.pulseWidth = TIMER2_BASE->CCR4 - ch4.startPulse;
    if (ch4.pulseWidth < 0) {
      ch4.pulseWidth += 65535;
    }
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC4P;
  }
}

void sigChange5() {
  //if channel is high
  if (0b1 & GPIOA_BASE->IDR >> 6) {
    //record start time and set edge detector trigger to falling
    ch5.startPulse = TIMER3_BASE->CCR1;
    TIMER3_BASE->CCER |= TIMER_CCER_CC1P;
  }
  else {
    //calculate pulse width and set edge detector trigger to raising
    ch5.pulseWidth = TIMER3_BASE->CCR1 - ch5.startPulse;
    if (ch5.pulseWidth < 0) {
      ch5.pulseWidth += 65535;
    }
    TIMER3_BASE->CCER &= ~TIMER_CCER_CC1P;
  }
}


void sigChange6() {
  //if channel is high
  if (0b1 & GPIOA_BASE->IDR >> 7) {
    //record start time and set edge detector trigger to falling
    ch6.startPulse = TIMER3_BASE->CCR2;
    TIMER3_BASE->CCER |= TIMER_CCER_CC2P;
  }
  else {
    //calculate pulse width and set edge detector trigger to raising
    ch6.pulseWidth = TIMER3_BASE->CCR2 - ch6.startPulse;
    if (ch6.pulseWidth < 0) {
      ch6.pulseWidth += 65535;
    }
    TIMER3_BASE->CCER &= ~TIMER_CCER_CC2P;
  }
}

