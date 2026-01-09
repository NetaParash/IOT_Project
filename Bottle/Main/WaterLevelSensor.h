/*
 * - Water levels are scanned from Bottom (Level 1) to Top (Level 4).
 * * * WATER SENSORS (Top to Bottom):
 * L4: GPIO 12  |  L3: GPIO 13  |  L2: GPIO 15  |  L1: GPIO 4
 */

#include <Arduino.h>
#include "GyroSensor.h"
#include <vector>

// ==========================================
// CONFIGURATION
// ==========================================

// Sensitivity for water (Lower value = less sensitive)
const int TOUCH_THRESHOLD = 50; 

class WaterProbe {
private:
    uint8_t _pin;
    int _threshold;
    int _levelIndex;

public:
    WaterProbe(uint8_t pin, int threshold, int levelIndex) 
        : _pin(pin), _threshold(threshold), _levelIndex(levelIndex) {}

    void setup() {
        // No setup for touch pin
    }

    bool isWet() {
        return touchRead(_pin) < _threshold;
    }

    int getLevelIndex() { return _levelIndex; }
    int getRawReading() { return touchRead(_pin); }
};

// ==========================================
// CLASS: WaterLevelSensor
// ==========================================
class WaterLevelSensor {
private:
    std::vector<WaterProbe> _probes;

public:
    WaterLevelSensor(std::vector<int> pins) {
        for (int i=0; i < pins.size(); i++) {
            _probes.push_back(WaterProbe(pins[i], TOUCH_THRESHOLD, i+1));
        }
    }

    void setup() {
        for (auto& probe : _probes) {
            probe.setup();
        }
        Serial.println("System Initialized.");
    }

    int getWaterLevel() {
        int foundLevel = 0;
        
        for (int i = 0; i < _probes.size(); i++) {
            if (!_probes[i].isWet()) {
                break;
            }
            foundLevel = _probes[i].getLevelIndex();
        }
        
        // printStatus(foundLevel);
        return (foundLevel * 100) / _probes.size();
    }

    void printStatus(int level) {
        if (level == 0) {
            Serial.println("Level: EMPTY (0%)");
        } else {
            float percentage = (float(level) / _probes.size()) * 100.0;
            Serial.print("Level: ");
            Serial.print(level);
            Serial.print(" (");
            Serial.print(percentage, 1);
            Serial.println("%)");
        }
    }
    
    // Debug tool
    void debugRaw() {
         for (auto& probe : _probes) {
            Serial.print(probe.getRawReading());
            Serial.print(" ");
         }
         Serial.println();
    }
};
