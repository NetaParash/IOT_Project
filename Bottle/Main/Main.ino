#include <Arduino.h>
#include "WaterLevelSensor.h"
#include "Screen.h"
#include "GyroSensor.h"
#include "ButtonInput.h"
#include <deque>


using namespace std;

// ========================
// WATER LEVEL PINS (Bottom → Top)
// ========================
WaterLevelSensor waterLevelSensor({15, 32, 33, 13});

// ========================
// OLED SCREEN
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
const unsigned long WATER_INTERVAL_MS = 100;

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
deque<int> lastWaterLevel;
const int BOTTLE_ML = 500;

// ==========================================
// MAIN SETUP
// ==========================================


void setup() {
  Serial.begin(115200);
  delay(1000);

  waterLevelSensor.setup();
}

void loop() {
    waterLevelSensor.getWaterLevel();
    waterLevelSensor.debugRaw();
    delay(1000);
}