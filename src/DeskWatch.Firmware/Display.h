#pragma once

#include <Arduino.h>

bool displayInit();
void displayWrite(const String& text);

void displayTurnOff();
void displayTurnOn();