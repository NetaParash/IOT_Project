 #include <Arduino.h>
 #include "WaterLevelSensor.h"
 #include "Screen.h"
 #include "GyroSensor.h"
 #include "ButtonInput.h"
 #include "LightNotifier.h"
 #include "TestAppClient.h"
 #include "BottleMode.h"
 #include <deque>
 #include "AppClient.h"

 using namespace std;

 // ========================
 // MODES
 // ========================
 #define MODE_COUNT 4

 static const BottleMode modes[MODE_COUNT] = {
         BottleMode("hydration", 2500, 60),
         BottleMode("sport",     3500, 30),
         BottleMode("office",    2000, 90),
         BottleMode("night",     500,  0)
 };

 // Initialize with mode HYDRATION (index 0)
 BottleMode currentMode(modes[0].name, modes[0].dailyGoal, modes[0].alertEveryMinutes);

 // ========================
 // APPLICATION
 // ========================
AppClient appClient(
    "Pura Vida", //"OrZ iPhone",
    "L&Y26100612", //"g0iibm9ik7ry",
    "https://iot-project-6i3k.onrender.com"
);

const unsigned long SETTINGS_PULL_INTERVAL_MS = (30 * 10000); // 30 seconds
unsigned long lastSettingsPullMs = 0;

const unsigned long WATER_SEND_EVENT_INTERVAL_MS = (10 * 10000); // 1 seconds
unsigned long lastWaterSendEventMs = 0;

 // ========================
 // WATER LEVEL PINS (Bottom -> Top)
 // ========================
 WaterLevelSensor waterLevelSensor({15, 32, 33, 13});

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
 Button btnSelect(18);

 // ========================
 // LIGHTS
 // ========================
 LightNotifier lights(12, 3);

 // ========================
 // TIMING
 // ========================
 unsigned long lastWaterCheck = 0;
 unsigned long lastNotificationTime = 0;
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

     lights.setup();
     btnNext.setup();
     btnSelect.setup();
     gyro.setup();
     waterLevelSensor.setup();
     lastNotificationTime = millis();

     testAppClientDrainBottle();

     screen.print("Pulling drinking data from application");
     totalDrankML = appClient.getLastTotalDrank();

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
     // GET SETTINGS
     // =====================================================
    if (now - lastSettingsPullMs >= SETTINGS_PULL_INTERVAL_MS) {
        lastSettingsPullMs = now;

        Serial.println("[MAIN] Pulling settings from backend");

        auto settings = appClient.getSettings();

        if (settings.size() == 3) {
            currentMode.name = settings[0];
            currentMode.dailyGoal = settings[1].toInt();
            currentMode.alertEveryMinutes = settings[2].toInt();

            Serial.println("[MAIN] Settings updated:");
            Serial.print("  mode: ");
            Serial.println(currentMode.name);
            Serial.print("  goal: ");
            Serial.println(currentMode.dailyGoal);
            Serial.print("  alerts_every: ");
            Serial.println(currentMode.alertEveryMinutes);

            
        } else {
            Serial.println("[MAIN] Failed to fetch settings");
        }
    }
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
                 // Update mode, goal and alert interval
                 currentMode.name = modes[browsingModeIndex].name;
                 currentMode.dailyGoal = modes[browsingModeIndex].dailyGoal;
                 currentMode.alertEveryMinutes = modes[browsingModeIndex].alertEveryMinutes;
                 currentScreen = STATE_HOME;

                 // Send new setting to the application
                 appClient.sendSettings(currentMode.name, currentMode.dailyGoal, currentMode.alertEveryMinutes);

                 // Reset the notifications timer on mode change
                 lastNotificationTime = now;
             }
             else if (btnNext.wasPressed()) {
                 browsingModeIndex++;
                 if (browsingModeIndex >= MODE_COUNT) {
                     currentScreen = STATE_HOME;
                 }
             }
             break;
     }

     // =====================================================
     // NOTIFICATION
     // =====================================================

     if (currentMode.alertEveryMinutes > 0) {
         if (now - lastNotificationTime >= currentMode.getAlertIntervalMs()) {
             // Notify
             lights.blinkNotification();
             screen.print("Time to drink!");
             // Reset timer
             lastNotificationTime = now;
         }
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

             // Send water level reading to the application (once in every fixed time interval)
             if (now - lastWaterSendEventMs >= WATER_SEND_EVENT_INTERVAL_MS) {
                 lastWaterSendEventMs = now;
                 appClient.sendEvent(totalDrankML, waterML);
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
                     screen.showHome(currentMode.name, currentMode.dailyGoal, totalDrankML, waterML);
                     break;
                 }

                 case STATE_SELECT_MODE: {
                     String selectModeName = modes[browsingModeIndex].name;
                     int selectModeGoal = modes[browsingModeIndex].dailyGoal;
                     screen.showModeMenu(selectModeName, selectModeGoal);
                     break;
                 }
             }
         }
     }
 }
