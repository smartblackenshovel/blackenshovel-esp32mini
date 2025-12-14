#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "gnss/GNSSController.h"
#include "imu/IMUController.h"
#include "http/HTTPHandler.h"
#include "utils/JsonUtils.h"
#include "SessionManager.h"
#include "dataAccess/SerialPortWriter.h"
#include "dataAccess/SerialPortReader.h"


#define LED_PIN 48
#define GNSS_RX_PIN 2
#define GNSS_TX_PIN 1
#define CYD_RX_PIN 6
#define CYD_TX_PIN 5

#define HTTP_ACTIVE 1

WiFiClientSecure securedClient;
const char* ssid = "iPhone de Lauro";
const char* password = "lolo1234";
char url[] = "https://short-teams-worry.loca.lt";

String organizationId;

char endpointOrganizations[] = "/organizations";
char endpointShovels[] = "/shovels";
char endpointUsers[] = "/users";
char endpointSpots[] = "/spots";
char endpointSessions[] = "/sessions";
char endpointSpotLogs[] = "/spot_logs";
char endpointSessionLogs[] = "/session_logs";
char mapEndpoint[] = "/map?lat=47.2229&lon=8.8169";

String shovelSerialNumber = "BS-#13823429-02";
String shovelId;
String sessionId;

HTTPHandler httpHandler(url);

GNSSController gnss(Serial1);
IMUController imu;

SerialPortWriter serialPortWriterCYD(Serial2);
SessionManager sessionManager(serialPortWriterCYD, httpHandler, gnss, imu);

SerialPortReader serialPortReaderCYD(Serial2, sessionManager);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN);
  Serial2.begin(9600, SERIAL_8N1, CYD_RX_PIN, CYD_TX_PIN);

  Serial.println("Initializing IMU...");
  while(!imu.begin()) {
      Serial.println("IMU not ready. Still initializing...");
      delay(100);
  }

  //-------------------------------------------------------------------------------------
  #if HTTP_ACTIVE
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  securedClient.setInsecure();
  
  //-------------------------------------------------------------------------------------

  Serial.println("Syncing time...");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); 
  Serial.println("Waiting for time...");
  time_t now;
  while ((now = time(NULL)) < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Time initialized.");

  //-------------------------------------------------------------------------------------
  
  sessionManager.beginSession();
  #endif
}

void loop() {
  serialPortReaderCYD.read();
  sessionManager.updateSession();

  if (sessionManager.getSessionUser() != nullptr && shovelId) {
    JsonDocument sessionPayload;
    sessionPayload["user_id"] = sessionManager.getSessionUser()->getId();
    sessionPayload["shovel_id"] = shovelId;

    HTTPResponse sessionResponse = httpHandler.post(
      endpointSessions,
      sessionPayload
    );

    if (sessionResponse.isSuccess()) {
      Serial.println("Session created successfully.");
      Serial.println(sessionResponse.getContent());
      sessionId = extractValues(sessionResponse.getContentAsJson(), "id")[0];
    } else {
      Serial.printf("Failed to create session. Status code: %d\n", sessionResponse.getStatusCode());
      return;
    }
  }

  if (sessionId) {
    // serialPortWriterCYD.writeIMU
    // serialPortWriterCYD.writeUserLocation
    // serialPortWriterCYD.writeSpotLocation
  }

  JsonDocument docImu;
  docImu["msgType"] = "imu";
  docImu["data"]["accel"]["x"] = accel.getX();
  docImu["data"]["accel"]["y"] = accel.getY();
  docImu["data"]["accel"]["z"] = accel.getZ();
  docImu["data"]["gyro"]["x"] = gyro.getX();
  docImu["data"]["gyro"]["y"] = gyro.getY();
  docImu["data"]["gyro"]["z"] = gyro.getZ();
  serializeJson(docImu, Serial2);

  JsonDocument docUserLoc;
  docUserLoc["msgType"] = "userLoc";
  docUserLoc["data"]["lat"] = loc.getLatitude();
  docUserLoc["data"]["lon"] = loc.getLongitude();
  serializeJson(docUserLoc, Serial2);

  JsonDocument docUsers;
  docUsers["msgType"] = "users";
  for (size_t i = 0; i < sessionManager.getUsers().size(); i++) {
    docUsers["data"][i]["name"] = sessionManager.getUsers()[i].getName();
    docUsers["data"][i]["id"] = sessionManager.getUsers()[i].getId();
  }
  serializeJson(docUsers, Serial2);

}
