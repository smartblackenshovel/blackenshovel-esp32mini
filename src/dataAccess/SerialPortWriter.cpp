#include "utils/JsonUtils.h"
#include "SerialPortWriter.h"

void SerialPortWriter::writeUsers(std::vector<User>& users) {
    JsonDocument docUsers;
    docUsers["msgType"] = "users";
    for (size_t i = 0; i < users.size(); i++) {
        docUsers["data"][i]["name"] = users[i].getName();
        docUsers["data"][i]["id"] = users[i].getId();
    }
    serializeJson(docUsers, serialPort);
}

void SerialPortWriter::writeImu(const IMU& imu) {
    JsonDocument docImu;
    docImu["msgType"] = "imu";
    docImu["data"]["accel"]["x"] = imu.getAccelerometer().getX();
    docImu["data"]["accel"]["y"] = imu.getAccelerometer().getY();
    docImu["data"]["accel"]["z"] = imu.getAccelerometer().getZ();
    docImu["data"]["gyro"]["x"] = imu.getGyroscope().getX();
    docImu["data"]["gyro"]["y"] = imu.getGyroscope().getY();
    docImu["data"]["gyro"]["z"] = imu.getGyroscope().getZ();
    serializeJson(docImu, serialPort);
}

void SerialPortWriter::writeUserLoc(const Location& loc) {
    JsonDocument docUserLoc;
    docUserLoc["msgType"] = "userLoc";
    docUserLoc["data"]["lat"] = loc.getLatitude();
    docUserLoc["data"]["lon"] = loc.getLongitude();
    serializeJson(docUserLoc, serialPort);

}

void SerialPortWriter::writeSpotLoc(const Location& loc) {
    JsonDocument docSpotLoc;
    docSpotLoc["msgType"] = "spotLoc";
    docSpotLoc["data"]["lat"] = loc.getLatitude();
    docSpotLoc["data"]["lon"] = loc.getLongitude();
    serializeJson(docSpotLoc, serialPort);
}