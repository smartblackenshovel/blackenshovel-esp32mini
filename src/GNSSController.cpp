#include <TinyGPSPlus.h>

TinyGPSPlus gps;

void gps() {
    while (Serial1.available() > 0) {
        gps.encode(Serial1.read());
        if (gps.location.isUpdated()) {
            Serial.print("Latitude= "); 
            Serial.print(gps.location.lat(), 6); 
            Serial.print(" Longitude= "); 
            Serial.println(gps.location.lng(), 6);
        }
    }
}
