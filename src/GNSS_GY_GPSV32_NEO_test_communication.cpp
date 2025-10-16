#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

HardwareSerial GPSserial(1);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  GPSserial.begin(9600, SERIAL_8N1, 1, 2); // RX=1, TX=2

  Serial.println("GPS reading in progress");
}

void loop() {
  while (GPSserial.available() > 0) {
    gps.encode(GPSserial.read());

    if (gps.location.isUpdated()) {
      Serial.print("Lat: "); Serial.print(gps.location.lat(), 6);
      Serial.print(", Lon: "); Serial.print(gps.location.lng(), 6);
      Serial.print(", Alt: "); Serial.print(gps.altitude.meters());
      Serial.print(" m, Speed: "); Serial.print(gps.speed.kmph());
      Serial.println(" km/h");
    }

    if (gps.time.isUpdated()) {
      Serial.print("Time (UTC): ");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());
    }
  }
}