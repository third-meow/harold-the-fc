// Test code for Adafruit Ultimate GPS
#include <Adafruit_GPS.h>

//GPS nicknamed pigeon
Adafruit_GPS pigeon(&Serial1);

#define GPSECHO  false
#define FASTLOOPTARGET 15
#define SLOWLOOPTARGET 2000

float lat;
float lng;
float altitude;
uint8_t sats;
uint8_t fixq;

uint16_t updateStartTime;
uint16_t updateTime;
uint16_t updateTimeMax;
uint16_t readStartTime;
uint16_t readTime;
uint16_t readTimeMax;

uint16_t fastLoopLength;
uint16_t fastLoopStart = 0;
uint16_t slowLoopLength;
uint16_t slowLoopStart = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}
  GPSinit();
  Serial.println("Setup complete");
}

void loop()
{
  uint16_t fastLoopEnd = micros();
  fastLoopLength = fastLoopEnd - fastLoopStart;

  if (fastLoopLength > FASTLOOPTARGET)
  {

    fastLoopStart = fastLoopEnd;
    delay(2);
    //readStartTime = micros();
    readGPS();

    /*readTime = (micros() - readStartTime);
      if (readTime > readTimeMax) {
      readTimeMax = readTime;
      Serial.print(readTimeMax); Serial.print(" : "); Serial.println(updateTimeMax);
      }*/
  }

  uint16_t slowLoopEnd = millis();
  slowLoopLength = slowLoopEnd - slowLoopStart;

  if (slowLoopLength > SLOWLOOPTARGET) {

    slowLoopStart = slowLoopEnd;

    //updateStartTime = micros();
    updateGPS(&lat, &lng, &altitude, &sats, &fixq);
    /*updateTime = (micros() - updateStartTime);
      if (updateTime > updateTimeMax) {
      updateTimeMax = updateTime;
      Serial.print(readTimeMax); Serial.print(" : "); Serial.println(updateTimeMax);
      }*/
    Serial.print(lat);
    Serial.print("\t");
    Serial.print(lng);
    Serial.print("\t");
    Serial.print(altitude);
    Serial.print("\t");
    Serial.print(sats);
    Serial.print("\t");
    Serial.println(fixq);


  }
}

void GPSinit() {
  //start GPS
  pigeon.begin(9600);
  //set baud rate to 115200
  pigeon.sendCommand("$PMTK251,115200*2C");
  //set to send both sentances
  pigeon.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  //set the update rate to once a second
  pigeon.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  //turn antenna status off
  pigeon.sendCommand("$PGCMD,33,0*6D");
  delay(300);
  //ask for firmware version
  Serial1.println(PMTK_Q_RELEASE);
}

void updateGPS(float *lat, float *lng, float *alt, uint8_t *sats, uint8_t *fixq) {
  if (pigeon.newNMEAreceived()) {
    if (!pigeon.parse(pigeon.lastNMEA()))
      return;
  }
  if (pigeon.fix) {
    *lat = pigeon.latitudeDegrees;
    *lng = pigeon.longitudeDegrees;
    *alt = pigeon.altitude;
    *sats = pigeon.satellites;
    *fixq = pigeon.fixquality;
  } else {
    *lat = -1;
    *lng = -1;
    *alt = -1;
    *sats = -1;
    *fixq = -1;
  }
}

void readGPS() {
  char c = pigeon.read();
}

