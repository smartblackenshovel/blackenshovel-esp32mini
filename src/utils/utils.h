#pragma once
#include "Arduino.h"

String currentTime();

#define DEBUG 1

#if DEBUG
template <typename T>
void logMsg(T msg) {
    if (Serial) Serial.println(msg);
}
#else
template <typename T>
void logMsg(T) {}  // versão vazia se DEBUG=0
#endif
