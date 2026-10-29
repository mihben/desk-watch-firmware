#pragma once

#include <Arduino.h>

void displayInit();
void displayWrite(const String& text);

void displayTurnOff();
void displayTurnOn();