#include <Arduino.h>
#include <vector>
#include <driver/touch_pad.h>

// ==========================================
// CALIBRATION (Based on your CSV)
// ==========================================

// 1. WET vs DRY
// When reading drops below this, we consider the pad "Active"
const int THRESH_WET = 680;

// 2. LOW HALF vs HIGH HALF
// When reading drops below this, we are in the "Top Half" of the pad
const int THRESH_MID_START = 600;
const int THRESH_MID_STOP  = 630; // Must rise above this to go back to "Low Half"

// ==========================================
// CLASS: WaterProbe
// ==========================================
class WaterProbe {
private:
    uint8_t _pin;
    touch_pad_t _touchPadIndex;

    // Internal States
    bool _isWet;       // Is water touching this pad?
    bool _isTopHalf;   // Is water in the upper 50% of this pad?

public:
    WaterProbe(uint8_t pin)
            : _pin(pin), _isWet(false), _isTopHalf(false) {
        _touchPadIndex = (touch_pad_t) digitalPinToTouchChannel(pin);
    }

    void setup() {
        touch_pad_config(_touchPadIndex, 0);
    }

    // Updates the internal state based on latest reading
    void update() {
        uint16_t val;
        touch_pad_read_filtered(_touchPadIndex, &val);

        // --- LOGIC 1: Detect Wet/Dry ---
        if (!_isWet && val < THRESH_WET) {
            _isWet = true;
        } else if (_isWet && val > THRESH_WET) {
            _isWet = false;
            _isTopHalf = false; // Reset half state if dry
        }

        // --- LOGIC 2: Detect Half (Only if Wet) ---
        if (_isWet) {
            if (!_isTopHalf && val < THRESH_MID_START) {
                _isTopHalf = true;
            } else if (_isTopHalf && val > THRESH_MID_STOP) {
                _isTopHalf = false;
            }
        }
    }

    bool isWet() { return _isWet; }
    bool isTopHalf() { return _isTopHalf; }

    int getRawReading() {
        uint16_t val;
        touch_pad_read_filtered(_touchPadIndex, &val);
        return val;
    }
};

// ==========================================
// CLASS: WaterLevelSensor
// ==========================================
class WaterLevelSensor {
private:
    std::vector<WaterProbe> _probes;

public:
    WaterLevelSensor(std::vector<int> pins) {
        for (int pin : pins) {
            _probes.push_back(WaterProbe(pin));
        }
    }

    void setup() {
        touch_pad_init();
        // High Ref 2.7V gives the wide range (800-500) you observed
        touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
        touch_pad_filter_start(10);

        for (auto& probe : _probes) {
            probe.setup();
        }
        Serial.println("System Initialized (Split-Pad Logic).");
    }

    int getWaterLevel() {
        // 1. Update all probes first
        for (auto& probe : _probes) {
            probe.update();
        }

        // 2. Find the Highest Wet Probe
        int activeProbeIndex = -1;
        for (int i = 0; i < _probes.size(); i++) {
            if (_probes[i].isWet()) {
                activeProbeIndex = i;
            } else {
                // If this probe is dry, we assume water hasn't reached here.
                // We stop checking higher probes (assuming gravity works).
                break;
            }
        }

        // 3. Calculate Score
        // 0 = Empty
        // Each full probe below active = 2 points
        // Active probe contributes 1 (Low Half) or 2 (High Half)

        int score = 0;

        if (activeProbeIndex == -1) {
            score = 0; // Completely Empty
        } else {
            // Add 2 points for every fully submerged probe below the active one
            score += (activeProbeIndex * 2);

            // Add points for the active probe itself
            if (_probes[activeProbeIndex].isTopHalf()) {
                score += 2; // Top half logic
            } else {
                score += 1; // Bottom half logic
            }
        }

        // Total max score = (NumProbes * 2)
        // e.g., 4 probes = Max Score 8.
        return calculatePercentage(score);
    }

    int calculatePercentage(int score) {
        int maxScore = _probes.size() * 2;
        int percent = (score * 100) / maxScore;

        Serial.print("Score: "); Serial.print(score);
        Serial.print("/"); Serial.print(maxScore);
        Serial.print(" -> "); Serial.print(percent); Serial.println("%");

        return percent;
    }

    void debugRaw() {
        for (auto& probe : _probes) {
            Serial.print(probe.getRawReading());
            Serial.print("\t");
        }
        Serial.println();
    }
};