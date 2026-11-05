#include "Device.h"
#include "Display.h"
#include "Logger.h"
#include "DeviceState.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define TIMEOUT 10000

static DeviceState deviceState = DeviceState::NotStarted;

static unsigned long lastReceivedAt = 0;

void deviceInit() {
  Serial.begin(9600);

  if (!displayInit()) {
    logError(F("device.init.failed"), F("Display initialization failed."));
    return;
  }

  deviceState = DeviceState::Offline;

  logInfo(F("device.started"), F("Device started. Waiting for connection..."));
  displayWrite(F("OFFLN"));
}

void deviceConnect() {
  lastReceivedAt = millis();
  if (deviceState == DeviceState::NotStarted) {
    logError(F("connection.command.rejected"), F("Device has not been started."));
    return;
  } else if (deviceState == DeviceState::Connected) {
    logError(F("connection.command.rejected"), F("Device is already connected."));
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError(F("connection.command.rejected"), F("Device is in sleeping mode."));
    return;
  }

  deviceState = DeviceState::Connected;
  displayWrite(F("READY"));

  logInfo(F("connection.established"), F("Connection established"));
}

void deviceDisconnect() { 
  lastReceivedAt = millis();
  if (deviceState == DeviceState::NotStarted) {
    logError(F("disconnection.command.rejected"), F("Device is not started."));
    return;
  } else if (deviceState == DeviceState::Offline) {
    logError(F("disconnection.command.rejected"), F("Device is not connected."));
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError(F("disconnection.command.rejected"), F("Device is in sleeping mode."));
    return;
  }

  deviceState = DeviceState::Offline;
  displayWrite(F("OFFLN"));

  logInfo(F("connection.closed"), F("Connection closed by host"));
}

void deviceSleep() {  
  lastReceivedAt = millis();
  if (deviceState == DeviceState::NotStarted) {
    logError(F("sleep.command.rejected"), F("Device is not started."));
    return;
  } else if (deviceState == DeviceState::Connected) {
    logError(F("sleep.command.rejected"), F("Device is connected."));
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError(F("sleep.command.rejected"), F("Device is already sleeping."));
    return;
  }

  displayTurnOff();
  deviceState = DeviceState::Sleeping;

  logInfo(F("device.asleep"), F("Device entering sleep mode."));
}

void deviceWakeup() {
  lastReceivedAt = millis(); 
  if (deviceState == DeviceState::NotStarted) {
    logError(F("wakeup.command.rejected"), F("Device is not started."));
    return;
  } else if (deviceState != DeviceState::Sleeping) {
    logError(F("wakeup.command.rejected"), F("Device is not sleeping."));
    return;
  }

  displayTurnOn();
  deviceState = DeviceState::Offline;

  logInfo(F("device.wokeup"), F("Device woke up from sleep mode."));
  displayWrite(F("OFFLN")); 
}

void deviceSetTime(const String& time) {
  lastReceivedAt = millis();

  if (deviceState == DeviceState::NotStarted) {
    logError(F("set.command.rejected"), F("Device is not started."));
    return;
  } else if (deviceState == DeviceState::Offline) {
    logError(F("set.command.rejected"), F("Device is not connected."));
    return;
  } else if (deviceState == DeviceState::Sleeping) {
    logError(F("set.command.rejected"), F("Device is in sleep mode."));
    return;
  }

  if (time.length() == 0) {
    logError(F("set.command.rejected"), F("Time is missing."));
    return;
  } else if (time.length() > DISPLAY_MAX_TEXT_LENGTH) {
    logError(F("set.command.rejected"), F("Time is too long for the display."));
    return;
  }

  displayWrite(time);
  logInfo(F("time.synchronized"), F("Time has been synchronized"));
}

void deviceCheckTimeout() {
  if (deviceState != DeviceState::Connected) {
    return;
  }

  if ((millis() - lastReceivedAt) >= TIMEOUT) {
    logError(F("connection.lost"), F("Connection is lost."));
    deviceState = DeviceState::Offline;
    displayWrite(F("OFFLN"));
  }
}