#pragma once
#include <Arduino.h>
#include "imu/IMU.h"
#include "models/Location.h"
#include "models/Spot.h"
#include "models/User.h"

class SerialPortWriter {
    public:
        SerialPortWriter(HardwareSerial& serial) : serialPort(serial) {}
        void writeUsers(std::vector<User>& users);
        void writeImu(const IMU& imu);
        void writeUserLoc(const Location& loc);
        void writeSpot(const Spot& loc);
    private:
        HardwareSerial& serialPort;
};