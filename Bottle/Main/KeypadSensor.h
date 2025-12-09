#ifndef KEYPAD_SENSOR_H
#define KEYPAD_SENSOR_H

#include <Arduino.h>
#include <Keypad.h>

enum class KeyEvent {
    NONE,
    NEXT,
    SELECT
};

class KeypadSensor {
private:
    Keypad keypad;
    KeyEvent lastEvent = KeyEvent::NONE;

public:
    // ❗ remove const — Keypad requires non-const byte* and char*
    KeypadSensor(char keys[4][3], byte rowPins[4], byte colPins[3])
        : keypad(makeKeymap(keys), rowPins, colPins, 4, 3)
    {}

    void update() {
        char k = keypad.getKey();
        if (k) {
            lastEvent = mapKey(k);
        }
    }

    bool hasEvent() {
        return lastEvent != KeyEvent::NONE;
    }

    KeyEvent getEvent() {
        KeyEvent e = lastEvent;
        lastEvent = KeyEvent::NONE;
        return e;
    }

private:
    KeyEvent mapKey(char k) {
        switch (k) {
            case '*': return KeyEvent::NEXT;
            case '#': return KeyEvent::SELECT;
            default:  return KeyEvent::NONE;
        }
    }

};

#endif
