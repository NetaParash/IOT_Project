#include <Arduino.h>
#include "ButtonInput.h"
#include "Screen.h"

// pin matching the Arduino example
const int buttonPin = 4;

// Create button object
Button button(buttonPin);

// Existing screen class from your project
Screen screen(Serial, 21, 22);

void setup() {
    Serial.begin(115200);

    button.setup();       // configure button
    screen.setup();       // init OLED

    screen.print("Button Test Ready");
}

void loop() {
    if (button.isPressed()) {
        screen.print("Pressed");
        Serial.println("Pressed");}
    else{
        screen.print("Not Pressed");
        Serial.println("Not Pressed");}
    delay(100);  // slow down screen flicker
}
