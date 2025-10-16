#include <Wire.h>
#include <Arduino_LSM6DSOX.h>

// Connects to IMU and displays real time the data (Accel, Gy, Temp) on PCs Serial Monitor
void setup() {
  Serial.begin(115200);
  while (!Serial);

  // initialisation of I2C on chosen pins (cf. WP2 Electronics Pins ):
  Wire.begin(4, 3); // SDA, SCL 
  
  if (!IMU.begin(Wire)) {
    Serial.println("Cannot find LSM6DSOX");
    while (1);
  }

  Serial.println("LMS6DOX found");
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float t;

  if (IMU.accelerationAvailable()) IMU.readAcceleration(ax, ay, az);
  if (IMU.gyroscopeAvailable()) IMU.readGyroscope(gx, gy, gz);
  if (IMU.temperatureAvailable()) IMU.readTemperature(t);

  Serial.print("Accel [m/s²]: ");
  Serial.print(ax); Serial.print(", ");
  Serial.print(ay); Serial.print(", ");
  Serial.print(az);
  
  Serial.print(" | Gyro [°/s]: ");
  Serial.print(gx); Serial.print(", ");
  Serial.print(gy); Serial.print(", ");
  Serial.print(gz);
  
  Serial.print(" | Temp: ");
  Serial.print(t);
  Serial.println(" °C");

  

  delay(500);
}