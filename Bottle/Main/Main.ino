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
    REGULAR, 
    SPORTS 
};

struct ModeConfig {
    String name;     
    int dailyGoal;   
    // TODO: Add time delta for notifications
};

static const ModeConfig modes[] = {
    // Name      // Goal (mL)
    { "Regular", 2000 },  // Index matches REGULAR
    { "Sports",  3000 }   // Index matches SPORTS
};

// ========================
// WATER LEVEL PINS (Bottom → Top)
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
Button btnNext(5);
Button btnSelect(4); 

// ========================
// TIMING
// ========================
unsigned long lastWaterCheck = 0;
const unsigned long WATER_INTERVAL_MS = 100;
const unsigned long SCREEN_REFRESH_RATE_MS = 100;

// ========================
// MENU STATE
// ========================
enum MenuState {
    STATE_HOME,
    STATE_SET_REGULAR,
    STATE_SET_SPORTS
};

MenuState currentScreen = STATE_HOME;
BottleMode activeMode = REGULAR;

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

    Wire.begin(21, 22); // SHARED I2C FOR SCREEN + MPU6050

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
    // UPDATE BUTTONS (required for wasPressed)
    // =====================================================
    btnSelect.update();   
    btnNext.update();

    // =====================================================
    // MENU NAVIGATION LOGIC (STATE MACHINE)
    // =====================================================
    switch (currentScreen) {
        // --- HOME SCREEN ---
        case STATE_HOME:
            if (btnNext.wasPressed()) {
                currentScreen = STATE_SET_REGULAR;
            }
            break;

        // --- SELECT REGULAR MODE ---
        case STATE_SET_REGULAR:
            if (btnSelect.wasPressed()) {
                activeMode = REGULAR; 
                currentScreen = STATE_HOME;
            } 
            else if (btnNext.wasPressed()) {
                currentScreen = STATE_SET_SPORTS;
            }
            break;

        // --- SELECT SPORTS MODE ---
        case STATE_SET_SPORTS:
            if (btnSelect.wasPressed()) {
                activeMode = SPORTS; 
                currentScreen = STATE_HOME;
            }
            else if (btnNext.wasPressed()) {
                currentScreen = STATE_HOME;
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
            return;
        }

        switch (currentScreen) {
            case STATE_HOME: {
                String modeName = modes[activeMode].name;
                int modeGoal = modes[activeMode].dailyGoal;
                screen.showHome(modeName, modeGoal, totalDrankML, waterML);
                break;
            }

            case STATE_SET_REGULAR:
                screen.showModeMenu(modes[REGULAR].name, modes[REGULAR].dailyGoal);
                break;

            case STATE_SET_SPORTS:
                screen.showModeMenu(modes[SPORTS].name, modes[SPORTS].dailyGoal);
                break;
        }
    }
}
