#include <Arduino.h>
#include "WaterLevelSensor.h"
#include "Screen.h"
#include "GyroSensor.h"
#include "ButtonInput.h"

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
// BUTTON
// ========================
Button button(4);

// ========================
// TIMING
// ========================
unsigned long lastWaterCheck = 0;
const unsigned long WATER_INTERVAL_MS = 10;

// ========================
// MENU STATE
// ========================
enum MenuScreen { WATER_SCREEN, DRANK_SCREEN };
MenuScreen currentScreen = WATER_SCREEN;

// ========================
// DRINK TRACKING
// ========================
int lastStableLevel = 0;
int totalDrankML = 0;
const int BOTTLE_ML = 500;

void setup() {
    Serial.begin(115200);
    delay(500);

    Wire.begin(21, 22); // SHARED I2C FOR SCREEN + MPU6050

    screen.setup();
    screen.print("Initializing...");

    button.setup();
    gyro.setup();
    waterLevelSensor.setup();

    screen.print("System Ready");
}

void loop() {
    unsigned long now = millis();

    // =====================================================
    // UPDATE BUTTON (required for wasPressed)
    // =====================================================
    button.update();

    // =====================================================
    // BUTTON PRESS → SWITCH SCREEN
    // =====================================================
    if (button.wasPressed()) {
        if (currentScreen == WATER_SCREEN)
            currentScreen = DRANK_SCREEN;
        else
            currentScreen = WATER_SCREEN;
    }

    // =====================================================
    // STABILITY CHECK
    // =====================================================
    bool stable = gyro.isStable();

    if (!stable) {
        String msg =
            "Please maintain\n"
            "the bottle stable!";
        screen.print(msg);
        return;
    }

    // =====================================================
    // SAMPLE WATER LEVEL EVERY 10ms
    // =====================================================
    if (now - lastWaterCheck >= WATER_INTERVAL_MS) {
        lastWaterCheck = now;

        int levelPercent = waterLevelSensor.getWaterLevel();
        int waterML = (levelPercent * BOTTLE_ML) / 100;

        // ===== Track drinking amount =====
        if (lastStableLevel > 0 && levelPercent < lastStableLevel) {

            int deltaPercent = lastStableLevel - levelPercent;
            int deltaML = (deltaPercent * BOTTLE_ML) / 100;

            totalDrankML += deltaML;

        }
        lastStableLevel = levelPercent;
            

        // =====================================================
        // MENU DRAWING
        // =====================================================
        if (currentScreen == WATER_SCREEN) {
            String text =
                "Water Level\n"
                + String(levelPercent) + "% (" + waterML + " ml)\n\n"
                "Press button to see\n"
                "Total Drank";
            screen.print(text);
        }
        else {
            String text =
                "Total Drank\n"
                + String(totalDrankML) + " ml\n\n"
                "Press button to see\n"
                "Water Level";
            screen.print(text);
        }

        // Debug output
        Serial.print("Level: ");
        Serial.print(levelPercent);
        Serial.print("%  Drank: ");
        Serial.print(totalDrankML);
        Serial.println(" ml");
    }
}
