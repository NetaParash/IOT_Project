/*
 * - Water levels are scanned from Bottom (Level 1) to Top (Level 4).
 */

#include <Arduino.h>
#include <vector>
#include <driver/touch_pad.h>

// ==========================================
// THRESHOLDS CONFIGURATION
// ==========================================

// 1. WET vs DRY
// When reading drops below this, we consider the pad "Active"
const int THRESH_WET = 680;

// 2. LOW HALF vs HIGH HALF
// When reading drops below this, we are in the "Top Half" of the pad
const int THRESH_MID_START = 600;
const int THRESH_MID_STOP  = 630; // Must rise above this to go back to "Low Half"

// ==========================================
// CLASS: WaterProbe (logic for a single touch pad for detecting if it touches water)
// ==========================================
class WaterProbe {
private:
    uint8_t _pin;
    touch_pad_t _touchPadIndex;

    bool _isWet;       // Is water touching this pad?
    bool _isTopHalf;   // Is water in the upper 50% of this pad?

public:
    WaterProbe(uint8_t pin) : _pin(pin), _isWet(false), _isTopHalf(false) {
        _touchPadIndex = (touch_pad_t) digitalPinToTouchChannel(pin);
    }

    void setup() {
        touch_pad_config(_touchPadIndex, 0);
    }

    // Updates the internal state based on latest reading
    void update() {
        uint16_t val;
        touch_pad_read_filtered(_touchPadIndex, &val);

        // --- Detect Wet/Dry ---
        if (!_isWet && val < THRESH_WET) {
            _isWet = true;
        } else if (_isWet && val > THRESH_WET) {
            _isWet = false;
            _isTopHalf = false; // Reset half state if dry
        }

        // --- Detect Half (Only if Wet) ---
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
// CLASS: WaterLevelSensor (logic for using all 4 touch sensors to get water level)
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
        touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
        touch_pad_filter_start(10);

        for (auto& probe : _probes) {
            probe.setup();
        }
        Serial.println("Touch Pads Initialized.");
    }

    int getWaterLevel() {
        // Update state of all probes
        for (auto& probe : _probes) {
            probe.update();
        }

        // Find the highest wet probe
        int activeProbeIndex = -1;
        for (int i = 0; i < _probes.size(); i++) {
            if (_probes[i].isWet()) {
                activeProbeIndex = i;
            } else {
                // If this probe is dry, we assume water hasn't reached here.
                // We stop checking higher probes (assuming the bottle is stable).
                break;
            }
        }

        // Calculate water level as "score" between 0 and 2n where n is the number of probes.
        int score = 0;

        if (activeProbeIndex == -1) {
            score = 0; // Bottle is empty
        } else {
            // Add 2 points for every fully wet probe below the active one
            score += (activeProbeIndex * 2);

            // Add 1 or 2 points for the active probe itself
            if (_probes[activeProbeIndex].isTopHalf()) {
                score += 2; // Top half is wet
            } else {
                score += 1; // Only bottom half is wet
            }
        }
        // Convert the score to a percentage between 0 and 100.
        return _calculatePercentage(score);
    }

    int _calculatePercentage(int score) {
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
