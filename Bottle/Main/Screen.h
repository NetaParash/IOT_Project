#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

class Screen {
private:
    HardwareSerial& serial;
    int sdaPin, sclPin;

    // Use the correct display class (SH1106G for 128x64)
    Adafruit_SH1106G display;

    const uint8_t OLED_ADDR = 0x3C;

public:
    Screen(HardwareSerial& s, int sda, int scl);

    void setup();
    void print(String text);
    void showHome(String modeName, int dailyGoal, int totalDrank, int currentLevel);
    void showModeMenu(String modeName, int dailyGoal);
};

#endif
