       
void writeLED(bool onoff) {
  digitalWrite(LED_INSIDE, !onoff);
}

void serialStartup(){
  for (int i = 0; i < 650; i++){
    delay(8);
    Serial.write(234); 
  }
  Serial.print("\n\n");
}
