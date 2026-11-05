#include "Logger.h"
#include "Device.h"
#include "Display.h"

#define COMMAND_CONNECT "CONNECT"
#define COMMAND_DISCONNECT "DISCONNECT"

#define COMMAND_SLEEP "SLEEP"
#define COMMAND_WAKEUP "WAKEUP"

#define COMMAND_SET "SET"

#define SEPARATOR " "

void setup() {
  deviceInit();
}

void loop() {  
  String command = "";
  String parameter = "";

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.indexOf(SEPARATOR) >= 0) {
      int index = input.indexOf(SEPARATOR);
      command = input.substring(0, index);
      parameter = input.substring(index + 1);
    }
    else {
      command = input;
    }

    if (command == COMMAND_CONNECT) {
      deviceConnect();
    } else if (command == COMMAND_DISCONNECT) {
      deviceDisconnect();
    } else if (command == COMMAND_SLEEP) {
      deviceSleep();
    } else if (command == COMMAND_WAKEUP) {
      deviceWakeup();
    } else if (command == COMMAND_SET) {
      deviceSetTime(parameter);
    } else {
      logError(F("command.unknown"), F("Unknown command: "), command);
    }
  } else {
    deviceCheckTimeout();
  }
} 