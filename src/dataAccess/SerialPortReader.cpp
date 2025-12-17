#include "utils/JsonUtils.h"
#include "utils/utils.h"
#include "SerialPortReader.h"

void SerialPortReader::read() {
    if (!serialPort.available()) {
        return;
    }
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, serialPort);
    if (error) {
        logMsg("Failed to parse JSON from Serial Port: ");
        logMsg(error.c_str());
        return;
    }
    String msgType = doc["msgType"].as<String>();
    msgType.trim();
    if (msgType == "selectedUser") {
        sessionUpdater.selectUser(doc["data"]["id"].as<String>());
        return;
    }
}
