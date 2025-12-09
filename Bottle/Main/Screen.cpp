#include "Screen.h"

Screen::Screen(HardwareSerial& s, int sda, int scl)
    : serial(s),
      sdaPin(sda),
      sclPin(scl),
      display(128, 64, &Wire)   // SH1106G constructor
{
}


void Screen::setup() {
    Wire.begin(sdaPin, sclPin);

    display.begin(OLED_ADDR, true);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    serial.println("Screen initialized.");
}

void Screen::print(String text) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(text);
    display.display();
}
