#include "Logger.h"

static void logBegin(const __FlashStringHelper* level, const __FlashStringHelper* event, const __FlashStringHelper* message) {
  Serial.print(F("{\"timestamp\":"));
  Serial.print(millis());
  Serial.print(F(",\"level\":\""));
  Serial.print(level);
  Serial.print(F("\",\"event\":\""));
  Serial.print(event);
  Serial.print(F("\",\"message\":\""));
  Serial.print(message);
}

static void logEnd() {
  Serial.println(F("\"}"));
}

void logInfo(const __FlashStringHelper* event, const __FlashStringHelper* message) {
  logBegin(F("INF"), event, message);
  logEnd();
}

void logError(const __FlashStringHelper* event, const __FlashStringHelper* message) {
  logBegin(F("ERR"), event, message);
  logEnd();
}

void logError(const __FlashStringHelper* event, const __FlashStringHelper* message, const String& detail) {
  logBegin(F("ERR"), event, message);
  Serial.print(detail);
  logEnd();
}
