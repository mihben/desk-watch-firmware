#include "Display.h"
#include "Logger.h"
#include "DeviceState.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define TIMEOUT 10000

DeviceState deviceState = DeviceState::NotStarted;

unsigned long lastReceivedAt = 0;

void deviceInit() {
  displayInit();

  Serial.begin(9600);
  deviceState = DeviceState::Offline;

  logInfo("device.started", "Device started. Waiting for connection...");
  displayWrite("OFFLN");
}

void deviceConnect() {
  lastReceivedAt = millis();
  if (deviceState == DeviceState::NotStarted) {
    logError("connection.command.rejected", "Device has not been started.");
    return;
  } else if (deviceState == DeviceState::Connected) {
    logError("connection.command.rejected", "Device is already connected.");
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError("connection.command.rejected", "Device is in sleeping mode.");
    return;
  }

  deviceState = DeviceState::Connected;
  displayWrite("READY");

  logInfo("connection.established", "Connection established");
}

void deviceDisconnect() { 
  lastReceivedAt = millis();
  if (deviceState == DeviceState::NotStarted) {
    logError("disconnection.command.rejected", "Device is not started.");
    return;
  } else if (deviceState == DeviceState::Offline) {
    logError("disconnection.command.rejected", "Device is not connected.");
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError("disconnection.command.rejected", "Device is in sleeping mode.");
    return;
  }

  deviceState = DeviceState::Offline;
  displayWrite("OFFLN");

  logInfo("connection.closed", "Connection closed by host");
}

void deviceSleep() {  
  lastReceivedAt = millis();
  if (deviceState == DeviceState::NotStarted) {
    logError("sleep.command.rejected", "Device is not started.");
    return;
  } else if (deviceState == DeviceState::Connected) {
    logError("sleep.command.rejected", "Device is connected.");
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError("sleep.command.rejected", "Device is already sleeping.");
    return;
  }

  displayTurnOff();
  deviceState = DeviceState::Sleeping;

  logInfo("device.asleep", "Device entering sleep mode.");
}

void deviceWakeup() {
  lastReceivedAt = millis(); 
  if (deviceState == DeviceState::NotStarted) {
    logError("wakeup.command.rejected", "Device is not started.");
    return;
  } else if (deviceState != DeviceState::Sleeping) {
    logError("wakeup.command.rejected", "Device is not sleeping.");
    return;
  }

  displayTurnOn();
  deviceState = DeviceState::Offline;

  logInfo("device.wokeup", "Device woke up from sleep mode.");
  displayWrite("OFFLN"); 
}

void deviceSetTime(String& time) {
  lastReceivedAt = millis();

  if (deviceState == DeviceState::NotStarted) {
    logError("set.command.rejected", "Device is not started.");
    return;
  } else if (deviceState == DeviceState::Offline) {
    logError("set.command.rejected", "Device is not connected.");
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError("set.command.rejected", "Device is in sleep mode.");
    return;
  }

  displayWrite(time);
  logInfo("time.synchronized", "Time has been synchronized");
}

void deviceCheckTimeout() {
  if (deviceState != DeviceState::Connected) {
    return;
  }

  if ((millis() - lastReceivedAt) >= TIMEOUT) {
    logError("connection.lost", "Connection is lost.");
    deviceState = DeviceState::Offline;
    displayWrite("OFFLN");
  }
}