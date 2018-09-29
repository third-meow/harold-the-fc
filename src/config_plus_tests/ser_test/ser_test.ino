
char latestByte;
String sentance = "";


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int16_t head = 360;
  uint8_t mode = 2;
  uint16_t height = 33010;
  float batv = 10.9;
  String outSentance = "";
  outSentance.concat('$');
  outSentance.concat(',');
  outSentance.concat(mode);
  outSentance.concat(',');
  outSentance.concat(head);
  outSentance.concat(',');
  outSentance.concat(height);
  outSentance.concat(',');
  outSentance.concat(batv);
  outSentance.concat(',');
  outSentance.concat('&');
  outSentance.concat(',');
  outSentance.concat('%');

  Serial1.print(outSentance);
  
  while(!Serial1.available()){};
  latestByte = Serial1.read();
  sentance += latestByte;
  if (latestByte == '%'){
    Serial.println(sentance);
    sentance = "";
  }
  delay(200);
}
