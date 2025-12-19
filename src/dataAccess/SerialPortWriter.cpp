#include "utils/JsonUtils.h"
#include "utils/utils.h"
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
    logMsg("Writing IMU Data to CYD");
    logMsg(imu.getAccelerometer().getX());
    logMsg(imu.getGyroscope().getX());

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
    logMsg("Writing User Location to CYD");
    logMsg(loc.getLatitude());
    logMsg(loc.getLongitude());
    logMsg(loc.getX());
    logMsg(loc.getY());

    JsonDocument docUserLoc;
    docUserLoc["msgType"] = "userLoc"; 
    docUserLoc["data"]["lat"] = loc.getLatitude();
    docUserLoc["data"]["lon"] = loc.getLongitude();
    docUserLoc["data"]["x"] = loc.getX();
    docUserLoc["data"]["y"] = loc.getY();
    serializeJson(docUserLoc, serialPort);

}

void SerialPortWriter::writeSpot(const Spot& spot) {
    logMsg("Writing Spot Location to CYD");
    logMsg(spot.getLocation().getLatitude());
    logMsg(spot.getLocation().getLongitude());
    logMsg(spot.getLocation().getX());
    logMsg(spot.getLocation().getY());

    JsonDocument docSpot;
    docSpot["msgType"] = "spot";
    docSpot["data"]["id"] = spot.getId();
    docSpot["data"]["lat"] = spot.getLocation().getLatitude();
    docSpot["data"]["lon"] = spot.getLocation().getLongitude();
    docSpot["data"]["x"] = spot.getLocation().getX();
    docSpot["data"]["y"] = spot.getLocation().getY();
    serializeJson(docSpot, serialPort);
}
