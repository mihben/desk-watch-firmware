#pragma once

#include <Arduino.h>

void deviceInit();

void deviceConnect();
void deviceDisconnect();

void deviceSleep();
void deviceWakeup();

void deviceSetTime(String& time);

void deviceCheckTimeout();