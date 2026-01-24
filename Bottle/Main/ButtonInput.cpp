#include "ButtonInput.h"

Button::Button(int pin)
    : pin(pin)
{}

void Button::setup() {
    pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return digitalRead(pin) == LOW;
}

void Button::update() {
    bool reading = digitalRead(pin);

    // (HIGH -> LOW)
    if (reading == LOW && lastState == HIGH) {
        pressedEvent = true;
    }

    lastState = reading;
}

bool Button::wasPressed() {
    if (pressedEvent) {
        pressedEvent = false;
        return true;
    }
    return false;
}
