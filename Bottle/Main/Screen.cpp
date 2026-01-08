#include "Screen.h"

Screen::Screen(HardwareSerial& s, int sda, int scl)
    : serial(s),
      sdaPin(sda),
      sclPin(scl),
      display(128, 64, &Wire)   // SH1106G constructor
{
}


void Screen::setup() {
    // Wire.begin(sdaPin, sclPin);

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

void Screen::showHome(String modeName, int dailyGoal, int totalDrank, int currentLevel) {
    int goalPercent = 0;
    if (dailyGoal > 0) {
        goalPercent = (totalDrank * 100) / dailyGoal;
    }

    String text = "Mode: " + modeName + "\n";
    text += "Goal: " + String(dailyGoal) + " mL\n";
    text += "Reached: " + String(goalPercent) + "% (" + String(totalDrank) + ")\n";
    text += "---------------------\n";
    text += "Level: " + String(currentLevel) + " mL";

    print(text);
}

void Screen::showModeMenu(String modeName, int dailyGoal) {
    String text = "Press Select to set:\n";
    text += modeName + " Mode\n";
    text += "(" + String(dailyGoal / 1000.0, 1) + " Liters)";
    
    print(text);
}