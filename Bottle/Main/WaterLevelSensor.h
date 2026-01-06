#include <Arduino.h>
#include <vector>
#include <driver/touch_pad.h> // REQUIRED for the hardware filter

// ==========================================
// CONFIGURATION
// ==========================================

// Threshold: The tipping point (calibrated for your current setup)
const int TOUCH_THRESHOLD = 50; 

// Hysteresis: The "Buffer Zone"
// Prevents flickering. Valid reading must go below 48 to be "Wet" 
// and rise above 52 to be "Dry".
const int HYSTERESIS = 2; 

class WaterProbe {
private:
    uint8_t _pin;
    touch_pad_t _touchPadIndex; // Internal ESP32 touch channel ID
    int _threshold;
    int _levelIndex;
    bool _currentState; // Tracks if we are currently Wet or Dry

public:
    WaterProbe(uint8_t pin, int threshold, int levelIndex) 
        : _pin(pin), _threshold(threshold), _levelIndex(levelIndex), _currentState(false) {
        
        // Convert the GPIO number (e.g., 4) to the Touch Pad Index (e.g., T0)
        // This is required for the low-level filtered reading function
        _touchPadIndex = (touch_pad_t) digitalPinToTouchChannel(pin);
    }

    void setup() {
        // This command physically connects the internal sensor to the pin.
        // Without this, the reading will always be 0.
        touch_pad_config(_touchPadIndex, 0);
    }

    // UPDATED: Now uses Hysteresis and Filtered Data
    bool isWet() {
        uint16_t filteredValue;
        
        // Read the SMOOTHED hardware value, not the raw instantaneous one
        touch_pad_read_filtered(_touchPadIndex, &filteredValue);

        // Hysteresis Logic:
        // If currently DRY, we need to drop significantly (below Threshold - Hysteresis) to become WET
        if (!_currentState && filteredValue < (_threshold - HYSTERESIS)) {
            _currentState = true;
        }
        // If currently WET, we need to rise significantly (above Threshold + Hysteresis) to become DRY
        else if (_currentState && filteredValue > (_threshold + HYSTERESIS)) {
            _currentState = false;
        }
        
        return _currentState;
    }

    int getLevelIndex() { return _levelIndex; }
    
    // Debug: Return the filtered value so you can calibrate easier
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
        for (int i=0; i < pins.size(); i++) {
            _probes.push_back(WaterProbe(pins[i], TOUCH_THRESHOLD, i+1));
        }
    }

    void setup() {
        // 1. INITIALIZE TOUCH HARDWARE
        touch_pad_init();
        
        // 2. SET VOLTAGE (Optional but recommended for stability)
        // High Ref 2.7V, Low Ref 0.5V, Atten 1V (Standard ESP32 default)
        touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

        // 3. CONFIGURE FILTER (The magic setting)
        // Period 10ms. This creates a moving average in hardware.
        // It kills the "5 unit" noise.
        touch_pad_filter_start(10); 
        
        // 4. SETUP PROBES
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
        
        printStatus(foundLevel);
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
    
    void debugRaw() {
         for (auto& probe : _probes) {
            Serial.print(probe.getRawReading());
            Serial.print("\t"); // Tab for easier reading in Serial Plotter
         }
         Serial.println();
    }
};