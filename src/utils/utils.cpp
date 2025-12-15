#include "utils.h"

String currentTime() {
  time_t now = time(NULL);
  struct tm* timeinfo = localtime(&now);
  if (timeinfo == nullptr) return "Time not set";
  char buffer[40];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
  return String(buffer);
}
