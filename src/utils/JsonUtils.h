#pragma once
#include <ArduinoJson.h>
#include <vector>
#include "models/User.h"
#include "models/Spot.h"

std::vector<User> parseUsers(JsonDocument& doc);
std::vector<Spot> parseSpots(JsonDocument& doc);

std::vector<String> extractValues(JsonDocument json, const String& key);

void debug(const JsonDocument& doc);
