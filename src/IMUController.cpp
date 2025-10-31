#include "IMUController.h"

IMUController::IMUController() : imu() {}

bool IMUController::begin() {
    imuWire = new TwoWire(0);
    imuWire->begin(4, 3);
    Serial.println("Initializing IMU...");
    if (!imuSensor.begin_I2C(106, imuWire)) {
        Serial.println("Failed to find LSM6DSOX sensor!");
        return false;
    }
    return true;
}

IMU IMUController::readIMU() {
    sensors_event_t accelEvent;
    sensors_event_t gyroEvent;

    imuSensor.getEvent(&accelEvent, &gyroEvent, nullptr);

    Accelerometer a(accelEvent.acceleration.x, accelEvent.acceleration.y, accelEvent.acceleration.z);
    Gyroscope g(gyroEvent.gyro.x, gyroEvent.gyro.y, gyroEvent.gyro.z);
    imu = IMU(a, g);
    return imu;
}
