#include <Arduino.h>
#include <FastLED.h>
#include "GNSSController.h"
#include "IMUController.h"


#define LED_PIN 48
#define GNSS_RX_PIN 2
#define GNSS_TX_PIN 1

GNSSController gnss(Serial1);
IMUController imuController;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN);

  Serial.println("Initializing IMU...");
  while(!imuController.begin()) {
      Serial.println("IMU not ready. Still initializing...");
      delay(100);
  }
}

void loop() {
  // Update GNSS location
    Location loc = gnss.updateLocation();
    
    // Read IMU
    IMU imu = imuController.read();

    // Print GNSS
    Serial.print("Latitude: ");
    Serial.print(loc.getLatitude(), 6);
    Serial.print(" Longitude: ");
    Serial.println(loc.getLongitude(), 6);

    // Print IMU accelerometer
    const Accelerometer& accel = imu.getAccelerometer();
    Serial.print("Accel X: "); Serial.print(accel.getX());
    Serial.print(" Y: "); Serial.print(accel.getY());
    Serial.print(" Z: "); Serial.println(accel.getZ());

    // Print IMU gyroscope
    const Gyroscope& gyro = imu.getGyroscope();
    Serial.print("Gyro X: "); Serial.print(gyro.getX());
    Serial.print(" Y: "); Serial.print(gyro.getY());
    Serial.print(" Z: "); Serial.println(gyro.getZ());

    delay(3000);
}
