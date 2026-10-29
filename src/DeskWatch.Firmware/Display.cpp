#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include "Logger.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define TEXT_SIZE 4

#define CURSOR_POS_X 6
#define CURSOR_POS_Y 0

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

void displayInit() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void displayWrite(const String& text) {
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(TEXT_SIZE);
  display.setCursor(CURSOR_POS_X, CURSOR_POS_Y);

  display.println(text);

  display.display();
}

void displayTurnOff() {
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void displayTurnOn() {
  display.ssd1306_command(SSD1306_DISPLAYON);
}