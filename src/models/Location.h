#pragma once

class Location {
    private:
        double latitude;
        double longitude;
        double x;
        double y;
    public:
        Location(double lat, double lon);
        double getLatitude() const { return latitude; }
        double getLongitude() const { return longitude; }
        double getX() const { return x; }
        double getY() const { return y; }

        double centerLat = 47.2229;
        double centerLon = 8.8169;
        double zoom = 17.5;
        double width = 240.0;
        double height = 320.0;
        double centerX, centerY;

        void latLonToPixels(double lat, double lon, double zoom, double &x, double &y);
};