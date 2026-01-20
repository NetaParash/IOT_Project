 #include <Arduino.h>
 #include "WaterLevelSensor.h"
 #include "Screen.h"
 #include "GyroSensor.h"
 #include "ButtonInput.h"
 #include "LightNotifier.h"
 #include "BottleMode.h"
 #include <deque>
 #include "AppClient.h"
 #include "Parameters.h"

 using namespace std;

 // ========================
 // APPLICATION
 // ========================
AppClient appClient(WIFI_NAME, WIFI_PASSWORD, APP_URL, BOTTLE_ID);

unsigned long lastSettingsPullMs = 0;
unsigned long lastWaterSendEventMs = 0;

 // ========================
 // WATER LEVEL PINS & THRESHOLDS (ordered Bottom -> Top)
 // ========================
 vector<int> touch_pads_pins = {32, 15, 33, 13};
// 4 thresholds per probe: To ensure stability, we use dual thresholds:
// Wet state: Drop below WET-LOW to activate, rise above WET-HIGH to clear.
// Mid-point: Drop below MID-LOW for "top half", rise above MID-HIGH for "bottom".
vector<vector<int>> thresholds = {
         // wet-low thresh, wet-high thresh, mid-low thresh, mid-high thresh
         {570, 630, 455, 470},
         {580, 615, 425, 445},
         {550, 610, 420, 435},
         {520, 560, 400, 420},
 };
 WaterLevelSensor waterLevelSensor(touch_pads_pins, thresholds);

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

 // ========================
 // MENU STATE
 // ========================
 enum MenuState {
     STATE_HOME,
     STATE_SELECT_MODE, // Generic state for browsing any mode
     STATE_RESET_DATA
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

 void setup() {
     Serial.begin(115200);
     delay(500);

     Wire.begin(21, 22);

     screen.setup();
     screen.attachAppClient(&appClient);
     screen.print("Initializing...");

     lights.setup();
     btnNext.setup();
     btnSelect.setup();
     gyro.setup();
     waterLevelSensor.setup();
     lastNotificationTime = millis();

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
                     currentScreen = STATE_RESET_DATA;
                 }
             }
             break;

         case STATE_RESET_DATA:
             if (btnNext.wasPressed()) {
                 currentScreen = STATE_HOME;
             }
             else if (btnSelect.wasPressed()) {
                 Serial.println("[MAIN] Clearing Water Data");
                 appClient.clearEventData();
                 totalDrankML = 0;

                 currentScreen = STATE_HOME;
             }
             break;
     }

     // =====================================================
     // NOTIFICATION
     // =====================================================

     if (currentMode.alertEveryMinutes > 0) {
         if (now - lastNotificationTime >= currentMode.getAlertIntervalMs()) {
             // Notify
             screen.print("Time to drink!");
             lights.blinkNotification();
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

             float levelPercent = waterLevelSensor.getWaterLevel();
             waterML = (levelPercent * BOTTLE_ML) / 100;

             lastWaterLevel.push_back(waterML);
             if (lastWaterLevel.size() == SLIDING_WINDOW_SIZE) {
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

                 case STATE_RESET_DATA: {
                     screen.showResetScreen();
                     break;
                 }
             }
         }
     }
 }
