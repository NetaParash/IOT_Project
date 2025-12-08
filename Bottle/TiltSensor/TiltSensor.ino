class TiltSensor {
private:
    uint8_t _pin;

public:
    TiltSensor(uint8_t pin) : _pin(pin) {}

    void setup() {
        pinMode(_pin, INPUT_PULLUP);
    }

    // In 10 consecutive measurements, if any is HIGH, returns true, and otherwise false.
    bool isTilted() {
        int samples = 10;
        for (int i = 0; i < samples; i++) {
            if (digitalRead(_pin) == HIGH) {
                return true;
            }
            delay(1);
        }
        return false;
    }
};