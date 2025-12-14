#include "utils/JsonUtils.h"
#include "SerialPortReader.h"

void SerialPortReader::read() {
    if (!serialPort.available()) {
        return;
    }
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, serialPort);
    if (error) {
        Serial.print("Failed to parse JSON from Serial Port: ");
        Serial.println(error.c_str());
        return;
    }
    String msgType = doc["msgType"].as<String>();
    msgType.trim();
    if (msgType == "selectedUser") {
        sessionUpdater.selectUser(doc["data"]["id"].as<String>());
        return;
    }
}
