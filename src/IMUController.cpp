#include "IMUController.h"

IMUController::IMUController() : imu() {}

bool IMUController::begin() {
    if (imuWire) {
        delete imuWire;
    }
    imuWire = new TwoWire(0);
    imuWire->begin(PIN_SDA, PIN_SCL);
    Serial.println("Initializing IMU...");
    if (!imuSensor.begin_I2C(106, imuWire)) {
        Serial.println("Failed to find LSM6DSOX sensor!");
        return false;
    }
    Serial.println("LSM6DSOX sensor found!");
    return true;
}

IMU IMUController::readIMU() {
    sensors_event_t accelEvent;
    sensors_event_t gyroEvent;
    sensors_event_t tempEvent;

    imuSensor.getEvent(&accelEvent, &gyroEvent, &tempEvent);

    Accelerometer a(accelEvent.acceleration.x, accelEvent.acceleration.y, accelEvent.acceleration.z);
    Gyroscope g(gyroEvent.gyro.x, gyroEvent.gyro.y, gyroEvent.gyro.z);
    imu = IMU(a, g);
    return imu;
}
