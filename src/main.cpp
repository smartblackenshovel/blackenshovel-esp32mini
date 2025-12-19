#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "gnss/GNSSController.h"
#include "imu/IMUController.h"
#include "http/HTTPHandler.h"
#include "utils/JsonUtils.h"
#include "utils/utils.h"
#include "SessionManager.h"
#include "dataAccess/SerialPortWriter.h"
#include "dataAccess/SerialPortReader.h"


#define LED_PIN 48
#define GNSS_RX_PIN 2
#define GNSS_TX_PIN 1
#define CYD_RX_PIN 6
#define CYD_TX_PIN 5

WiFiClientSecure securedClient;
const char* ssid = "";
const char* password = "";

char url[] = "";
HTTPHandler httpHandler(url);

GNSSController gnss(Serial1);
IMUController imu;

SerialPortWriter serialPortWriterCYD(Serial2);
SessionManager sessionManager(serialPortWriterCYD, httpHandler, gnss, imu);

SerialPortReader serialPortReaderCYD(Serial2, sessionManager);

void setup() {
  #if DEBUG
  Serial.begin(115200);
  #endif
  Serial1.begin(9600, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN);
  Serial2.begin(9600, SERIAL_8N1, CYD_RX_PIN, CYD_TX_PIN);

  logMsg("Initializing IMU...");
  while(!imu.begin()) {
      delay(100);
  }

  //-------------------------------------------------------------------------------------
  #if HTTP_ACTIVE
  logMsg("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  logMsg("");
  logMsg("WiFi connected");
  logMsg(WiFi.localIP());

  securedClient.setInsecure();
  
  //-------------------------------------------------------------------------------------

  logMsg("Syncing time...");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); 
  logMsg("Waiting for time...");
  time_t now;
  while ((now = time(NULL)) < 8 * 3600 * 2) {
    delay(500);
  }
  logMsg("Time initialized.");

  //-------------------------------------------------------------------------------------
  #endif
  sessionManager.beginSession();
}

void loop() {
  serialPortReaderCYD.read();
  sessionManager.updateSession();
  delay(1000);
}
