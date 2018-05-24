
void writeLED(bool onoff) {
  digitalWrite(LED_INSIDE, !onoff);
}

void serialStartup(){
  for (int i = 0; i < 350; i++){
    delay(10);
    Serial.write(234); 
  }
  Serial.print("\n\n");
}
