void initReciver(){
    //set all reciver input pins to input
  for (int i = 0; i < 4; i++) {
    pinMode(ch[i], INPUT);
  }
}

void updateReciver(int *throttle, int *yaw, int *pitch, int *roll) {
  *throttle = analogRead(ch[0]);
  
  int rudder = analogRead(ch[1]);
  *yaw = map(rudder, 0, 255, -MAXTILT, MAXTILT);
  
  int elevator = analogRead(ch[2]);
  *pitch = map(elevator, 0, 255, -MAXTILT, MAXTILT);
  
  int aileron = analogRead(ch[3]);
  *roll = map(aileron, 0, 255, -MAXTILT, MAXTILT);
}

