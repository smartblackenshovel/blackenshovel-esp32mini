#include "GNSSController.h"

GNSSController::GNSSController(HardwareSerial& serialPort) : serialPort(serialPort), location(0, 0) {}

Location GNSSController::getLocation() const {
    return location;
}

Location GNSSController::updateLocation() {
    while (serialPort.available()) {
        gps.encode(serialPort.read());
    }

    if (!gps.location.isUpdated()) {
        return location;
    }

    location = Location(gps.location.lat(), gps.location.lng());
    return location;
}
