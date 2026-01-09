#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    int pin;
    bool activeLow;
    bool lastReading = HIGH;
    bool lastStable = HIGH;
    bool pressedEvent = false;

    unsigned long lastDebounce = 0;
    static const unsigned long debounceDelay = 30;  // ms

public:
    // activeLow = true → button connected to GND + INPUT_PULLUP
    // activeLow = false → button connected to VCC + plain INPUT
    Button(int pin, bool activeLow = true)
        : pin(pin), activeLow(activeLow)
    {}

    void setup() {
        if (activeLow)
            pinMode(pin, INPUT_PULLUP);
        else
            pinMode(pin, INPUT);
    }

    // Must be called in loop()
    void update() {
        bool raw = digitalRead(pin);

        // Debouncing
        if (raw != lastReading) {
            lastDebounce = millis();
            lastReading = raw;
        }

        if ((millis() - lastDebounce) > debounceDelay) {
            if (raw != lastStable) {
                lastStable = raw;

                // detect rising edge of a "press"
                bool isPressed = activeLow ? (lastStable == LOW)
                                           : (lastStable == HIGH);

                if (isPressed) {
                    pressedEvent = true;
                }
            }
        }
    }

    // Returns true ONCE per press
    bool wasPressed() {
        if (pressedEvent) {
            pressedEvent = false;
            return true;
        }
        return false;
    }

    // Real-time pressed state (not latched)
    bool isPressed() {
        return activeLow ? (lastStable == LOW)
                         : (lastStable == HIGH);
    }
};

#endif
