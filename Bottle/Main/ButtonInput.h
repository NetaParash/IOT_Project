#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    int pin;

    // --- Added for edge detection ---
    bool lastState = false;      // previous reading
    bool pressedEvent = false;   // becomes true only on rising edge

public:
    Button(int pin);
    void setup();
    bool isPressed();     // HIGH when held
    bool wasPressed();    // NEW: true only once per click

    void update();        // NEW: must be called each loop
};

#endif
