#include <Arduino.h>

void log(const String& level, const String& event, const String& message) {
  Serial.print(F("{\"timestamp\":"));
  Serial.print(millis());
  Serial.print(F(",\"level\":\""));
  Serial.print(level);
  Serial.print(F("\",\"event\":\""));
  Serial.print(event);
  Serial.print(F("\",\"message\":\""));
  Serial.print(message);
  Serial.println(F("\"}"));
}

void logInfo(const String& event, const String& message) {
  log("INF", event, message);
}

void logError(const String& event, const String& message) {
  log("ERR", event, message);
}