#include <TinyGPSPlus.h>
#include "Location.h"

class GNSSController {
    private:
        TinyGPSPlus gps;
        HardwareSerial& serialPort;
        Location location;

    public:
        GNSSController(HardwareSerial& serialPort);
        Location updateLocation();
        Location getLocation() const;
};

