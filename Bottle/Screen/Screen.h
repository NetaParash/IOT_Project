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
    Adafruit_SH110X display;
    const uint8_t OLED_ADDR = 0x3C;

public:
    Screen(HardwareSerial& s, int sda, int scl);

    void setup();
    void print(String text);
};

#endif
