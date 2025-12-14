#pragma once
#include <ArduinoJson.h>
#include <vector>
#include "models/User.h"

std::vector<User> parseUsers(JsonDocument& doc);

std::vector<String> extractValues(JsonDocument json, const String& key);

void debug(const JsonDocument& doc);
