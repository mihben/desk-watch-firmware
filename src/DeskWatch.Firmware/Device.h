#pragma once

#include <Arduino.h>

void deviceInit();

void deviceConnect();
void deviceDisconnect();

void deviceSleep();
void deviceWakeup();

void deviceSetTime(const String& time);

void deviceCheckTimeout();