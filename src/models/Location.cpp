#include <cmath>
#include "Location.h"
#include "utils/utils.h"

Location::Location(double lat, double lon) : latitude(lat), longitude(lon) {
    latLonToPixels(centerLat, centerLon, zoom, centerX, centerY);
    latLonToPixels(lat, lon, zoom, x, y);

    double deltaX = x - centerX;
    double deltaY = y - centerY;

    x = (width  / 2.0) + deltaX;
    y = (height / 2.0) + deltaY;
}

void Location::latLonToPixels(double lat, double lon, double zoom, double &x, double &y) {
    double latRad = lat * M_PI / 180.0;
    double n = std::pow(2.0, zoom);

    x = ((lon + 180.0) / 360.0) * n * 256.0;

    double sec = 1.0 / std::cos(latRad);
    y = (1.0 - std::log(std::tan(latRad) + sec) / M_PI) / 2.0 * n * 256.0;
};
