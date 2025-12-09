#include <Arduino.h>
#include "KeypadSensor.h"
#include "Screen.h"

// Full 4×3 keypad layout — library requires this
char keys[4][3] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[4] = {25, 26, 27, 16};   // row 4 = 16, others = safe dummy GPIOs
byte colPins[3] = {17, 25, 18};       // only col 1=17 and col 3=18 real
int water = 0;


KeypadSensor keypadSensor(keys, rowPins, colPins);

Screen screen(Serial, 21, 22);

void setup() {
    Serial.begin(115200);
    screen.setup();
    screen.print("Keypad Ready");
}

void loop() {
    keypadSensor.update();

    if (keypadSensor.hasEvent()) {
        KeyEvent e = keypadSensor.getEvent();

        if (e == KeyEvent::NEXT) {
            Serial.println("NEXT");
            screen.print("NEXT");
        }
        if (e == KeyEvent::SELECT) {
            Serial.println("SELECT");
            screen.print("SELECT");
        }
    }
}
