#pragma once

#include <Arduino.h>

#define DISPLAY_MAX_TEXT_LENGTH 5

bool displayInit();
void displayWrite(const String& text);

void displayTurnOff();
void displayTurnOn();