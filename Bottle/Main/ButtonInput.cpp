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

// --- NEW: must be called each loop ---
void Button::update() {
    bool reading = digitalRead(pin);

    // Detect rising edge (HIGH -> LOW)
    if (reading == LOW && lastState == HIGH) {
        pressedEvent = true;
    }

    lastState = reading;
}

// --- NEW: returns true only once per press ---
bool Button::wasPressed() {
    if (pressedEvent) {
        pressedEvent = false;
        return true;
    }
    return false;
}
