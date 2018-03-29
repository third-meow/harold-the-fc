void setup() {
  Serial.begin(500000);
  pinMode(16, INPUT);
}

void loop() {
  Serial.println(pulseIn(16, HIGH));
}
