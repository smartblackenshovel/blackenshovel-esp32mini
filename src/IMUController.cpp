#include "IMUController.h"

IMUController::IMUController() : imu() {}

bool IMUController::begin() {
    if (imuWire) {
        delete imuWire;
    }
    imuWire = new TwoWire(0);
    imuWire->begin(PIN_SDA, PIN_SCL);
    
    if (!imuSensor.begin_I2C(106, imuWire)) {
        return false;
    }
    return true;
}

IMU IMUController::read() {
    sensors_event_t accelEvent;
    sensors_event_t gyroEvent;
    sensors_event_t tempEvent;

    imuSensor.getEvent(&accelEvent, &gyroEvent, &tempEvent);

    accel = Accelerometer(accelEvent.acceleration.x, accelEvent.acceleration.y, accelEvent.acceleration.z);
    gyro = Gyroscope(gyroEvent.gyro.x, gyroEvent.gyro.y, gyroEvent.gyro.z);
    imu = IMU(accel, gyro);
    return imu;
}
