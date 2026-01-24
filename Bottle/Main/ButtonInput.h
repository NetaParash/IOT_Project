#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    int pin;
    bool lastState = false;
    bool pressedEvent = false;

public:
    Button(int pin);
    void setup();
    bool isPressed();
    bool wasPressed();

    void update();
};

#endif
