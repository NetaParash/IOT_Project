#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

class Screen {
private:
    HardwareSerial& serial;
    int sdaPin, sclPin;
    Adafruit_SH110X display = Adafruit_SH110X(128, 64, &Wire);
    const uint8_t OLED_ADDR = 0x3C;

public:
    Screen(HardwareSerial& s, int sda, int scl)
        : serial(s), sdaPin(sda), sclPin(scl) {}

    void setup() {
        // Start I2C on given pins
        Wire.begin(sdaPin, sclPin);

        // Start OLED
        display.begin(OLED_ADDR, true);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0, 0);

        serial.println("Screen initialized.");
    }

    void print(String text) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.println(text);
        display.display();
    }
};
