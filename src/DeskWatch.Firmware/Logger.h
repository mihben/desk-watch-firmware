#pragma once

#include <Arduino.h>

void logInfo(const __FlashStringHelper* event, const __FlashStringHelper* message);
void logError(const __FlashStringHelper* event, const __FlashStringHelper* message);
void logError(const __FlashStringHelper* event, const __FlashStringHelper* message, const String& detail);
