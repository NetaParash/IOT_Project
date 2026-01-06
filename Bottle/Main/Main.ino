#include <Arduino.h>
#include "WaterLevelSensor.h"
#include "Screen.h"
#include "GyroSensor.h"
#include "ButtonInput.h"
#include <deque>

using namespace std;

// ========================
// MODES
// ========================
enum BottleMode {
    HYDRATION,
    SPORT,
    OFFICE,
    NIGHT,
    MODE_COUNT // Helper to know how many modes exist
};

struct ModeConfig {
    String name;
    int dailyGoal;
    // TODO: Add time delta for notifications
};

// Modes Configuration
static const ModeConfig modes[MODE_COUNT] = {
        { "Hydration", 2500 }, // Index 0
        { "Sport",     3500 }, // Index 1
        { "Office",    2000 }, // Index 2
        { "Night",     500  }  // Index 3
};

// ========================
// WATER LEVEL PINS (Bottom -> Top)
// ========================
WaterLevelSensor waterLevelSensor({14, 27, 33, 32});

// ========================
// OLED SCREEN (21/22 I2C)
// ========================
Screen screen(Serial, 21, 22);

// ========================
// GYRO SENSOR
// ========================
GyroSensor gyro(21, 22);

// ========================
// BUTTONS
// ========================
Button btnNext(4);
Button btnSelect(5);

// ========================
// TIMING
// ========================
unsigned long lastWaterCheck = 0;
const unsigned long WATER_INTERVAL_MS = 100;
const unsigned long SCREEN_REFRESH_RATE_MS = 150;

// ========================
// MENU STATE
// ========================
enum MenuState {
    STATE_HOME,
    STATE_SELECT_MODE // Generic state for browsing any mode
};

MenuState currentScreen = STATE_HOME;
BottleMode activeMode = HYDRATION;
int browsingModeIndex = 0; // Index to track which mode we are currently looking at in the menu

// ========================
// DRINK TRACKING
// ========================
int lastStableLevel = 0;
int totalDrankML = 0;
deque<int> lastWaterLevel;
int waterML = 0;
const int BOTTLE_ML = 500;

void setup() {
    Serial.begin(115200);
    delay(500);

    Wire.begin(21, 22);

    screen.setup();
    screen.print("Initializing...");

    btnNext.setup();
    btnSelect.setup();
    gyro.setup();
    waterLevelSensor.setup();

    screen.print("System Ready");
}

void loop() {
    unsigned long now = millis();

    // =====================================================
    // UPDATE BUTTONS
    // =====================================================
    btnSelect.update();
    btnNext.update();

    // =====================================================
    // MENU NAVIGATION LOGIC (STATE MACHINE)
    // =====================================================
    switch (currentScreen) {
        // --- HOME SCREEN ---
        case STATE_HOME:
            // If Select button was pressed in HOME screen, we "consume" the press to clear the memory
            btnSelect.wasPressed();

            if (btnNext.wasPressed()) {
                // Start browsing from the first mode in the list
                currentScreen = STATE_SELECT_MODE;
                browsingModeIndex = 0;
            }
            break;

            // --- MODE SELECTION MENU ---
        case STATE_SELECT_MODE:
            if (btnSelect.wasPressed()) {
                // Confirm selection
                activeMode = (BottleMode)browsingModeIndex;
                currentScreen = STATE_HOME;
            }
            else if (btnNext.wasPressed()) {
                // Move to next mode in the list
                browsingModeIndex++;

                // If we passed the last mode, go back to Home
                if (browsingModeIndex >= MODE_COUNT) {
                    currentScreen = STATE_HOME;
                }
            }
            break;
    }

    // =====================================================
    // STABILITY CHECK
    // =====================================================
    bool stable = gyro.isStable();

    // =====================================================
    // SAMPLE WATER LEVEL EVERY 10ms
    // =====================================================
    if (stable) {
        if (now - lastWaterCheck >= WATER_INTERVAL_MS) {
            lastWaterCheck = now;

            int levelPercent = waterLevelSensor.getWaterLevel();
            waterML = (levelPercent * BOTTLE_ML) / 100;

            lastWaterLevel.push_back(waterML);
            if (lastWaterLevel.size() == 10) {
                int all = lastWaterLevel.front();
                bool consistent = true;

                for(int m: lastWaterLevel) {
                    if(m != all) { consistent = false; break; }
                }
                if (consistent) {
                    if (lastStableLevel > all) {
                        totalDrankML += (lastStableLevel - all);
                    }
                    lastStableLevel = all;
                }
                lastWaterLevel.pop_front();
            }
        }
    }

    // =====================================================
    // MENU DRAWING
    // =====================================================
    static unsigned long lastDraw = 0;
    if (now - lastDraw > SCREEN_REFRESH_RATE_MS) {
        lastDraw = now;

        // Check stability first
        if (!stable && currentScreen == STATE_HOME) {
            screen.print("Please maintain\nthe bottle stable!");
        }
        else {
            switch (currentScreen) {
                case STATE_HOME: {
                    String modeName = modes[activeMode].name;
                    int modeGoal = modes[activeMode].dailyGoal;
                    screen.showHome(modeName, modeGoal, totalDrankML, waterML);
                    break;
                }

                case STATE_SELECT_MODE: {
                    // Show the mode we are currently BROWSING
                    String modeName = modes[browsingModeIndex].name;
                    int modeGoal = modes[browsingModeIndex].dailyGoal;
                    screen.showModeMenu(modeName, modeGoal);
                    break;
                }
            }
        }
    }
}
