                          
void writeLED(bool onoff) {
  digitalWrite(LED_INSIDE, !onoff);
}

void flashLED(uint8_t repeats, uint16_t dlay){
  for (int i = 0; i < repeats; i++){
    digitalWrite(LED_INSIDE, i % 2);
    delay(dlay);
  }
}


void serialStartup(){
  for (int i = 0; i < 350; i++){
    delay(10);
    Serial.write(234); 
  }
  Serial.print("\n\n");
}
