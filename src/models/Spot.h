#pragma once
#include <Arduino.h>
#include "Location.h"

class Spot {
    private:
        String id;
        Location loc;
    public:
        Spot(const String& spotId, double lat, double lon)
            : id(spotId), loc(lat, lon) {}
        String getId() const { return id; }
        void setLocation(double latitude, double longitude) {
            loc = Location(latitude, longitude);
        }
};
