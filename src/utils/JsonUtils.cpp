#include "JsonUtils.h"

std::vector<User> parseUsers(JsonDocument& doc) {
    std::vector<User> users;
    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
        String name = obj["name"].as<String>();
        String id = obj["id"].as<String>();
        users.push_back(User(name, id));
    } 
    return users;
}

std::vector<Spot> parseSpots(JsonDocument& doc) {
  std::vector<Spot> spots;
    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
        String id = obj["id"].as<String>();
        double lat = obj["latitude"].as<double>();
        double lon = obj["longitude"].as<double>();
        spots.push_back(Spot(id, lat, lon));
    } 
    return spots;
} 

std::vector<String> extractValues(JsonDocument json, const String& key) {
  std::vector<String> result;

  JsonArray arr = json.as<JsonArray>();
  for (JsonObject obj : arr) {
      result.push_back(String(obj[key].as<const char*>()));
  }

  return result;
}

void debug(const JsonDocument& doc) {
  String output;
  serializeJsonPretty(doc, output);
  Serial.println(output);
}
