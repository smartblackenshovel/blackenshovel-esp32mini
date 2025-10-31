#include <Wire.h>
#include <Adafruit_LSM6DSOX.h>
#include "IMU.h"

class IMUController {
    private:
        TwoWire* imuWire;
        Adafruit_LSM6DSOX imuSensor;
        Accelerometer accel;
        Gyroscope gyro;
        IMU imu;
        double accelX, accelY, accelZ;
        double gyroX, gyroY, gyroZ;

    public:
        IMUController();
        bool begin();
        IMU readIMU();
};
