#include <Arduino.h>
#include <FastLED.h>
#include "GNSSController.h"


#define LED_PIN 48
#define GNSS_RX_PIN 2
#define GNSS_TX_PIN 1

GNSSController gnss(Serial1);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN); // RX=2, TX=1
  while (!Serial) { ; }
}

void loop() {
  Location loc = gnss.updateLocation();
  Serial.print("Latitude: ");
  Serial.print(loc.getLatitude(), 6);
  Serial.print(" Longitude: ");
  Serial.println(loc.getLongitude(), 6);
  delay(3000);
}
