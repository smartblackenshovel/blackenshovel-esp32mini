#pragma once
#include <Arduino.h>
#include "models/User.h"
#include "imu/IMU.h"
#include "models/Location.h"

class SerialPortWriter {
    public:
        SerialPortWriter(HardwareSerial& serial) : serialPort(serial) {}
        void writeUsers(std::vector<User>& users);
        void writeImu(const IMU& imu);
        void writeUserLoc(const Location& loc);
        void writeSpotLoc(const Location& loc);
    private:
        HardwareSerial& serialPort;
};