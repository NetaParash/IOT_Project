#include "ButtonInput.h"

Button::Button(int pin)
    : pin(pin)
{}

void Button::setup() {
    pinMode(pin, INPUT);
}

bool Button::isPressed() {
    return digitalRead(pin) == HIGH;
}

// --- NEW: must be called each loop ---
void Button::update() {
    bool reading = digitalRead(pin);

    // Detect rising edge (LOW → HIGH)
    if (reading == HIGH && lastState == LOW) {
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
