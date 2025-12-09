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
