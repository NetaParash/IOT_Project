#include <Keypad.h>
enum class KeypadEvent {
    NONE,
    NEXT,
    SELECT
};

/********************************************
 *              KeypadSensor Class
 ********************************************/
class KeypadSensor {
private:
    Keypad* keypadPtr;
    KeypadEvent lastEvent = KeypadEvent::NONE;

    byte* rowPinsCopy;
    byte* colPinsCopy;
    int numRows;
    int numCols;

public:
    KeypadSensor(char* keymap,
                 int rowPins[], int colPins[],
                 int rCount, int cCount)
    {
        numRows = rCount;
        numCols = cCount;

        rowPinsCopy = new byte[numRows];
        for (int i = 0; i < numRows; i++)
            rowPinsCopy[i] = (byte)rowPins[i];

        colPinsCopy = new byte[numCols];
        for (int i = 0; i < numCols; i++)
            colPinsCopy[i] = (byte)colPins[i];

        keypadPtr = new Keypad(makeKeymap(keymap),
                               rowPinsCopy, colPinsCopy,
                               numRows, numCols);
    }

    void setup() {
        Serial.println("Keypad initialized");
    }

    // Convert raw char → enum event
    KeypadEvent mapKeyToEvent(char key) {
        switch (key) {
            case '1': return KeypadEvent::NEXT;
            case '2': return KeypadEvent::SELECT;
            default:  return KeypadEvent::NONE;
        }
    }

    void update() {
        char key = keypadPtr->getKey();
        if (key) {
            lastEvent = mapKeyToEvent(key);
        }
    }

    bool hasEvent() {
        return lastEvent != KeypadEvent::NONE;
    }

    KeypadEvent getLastEvent() {
        KeypadEvent e = lastEvent;
        lastEvent = KeypadEvent::NONE;
        return e;
    }
};
