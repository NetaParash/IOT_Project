#ifndef IOT_PROJECT_DEFINITIONS_H
#define IOT_PROJECT_DEFINITIONS_H

#include "secrets.h"

using namespace std;

const unsigned long SETTINGS_PULL_INTERVAL_MS = (30 * 1000); // 30 seconds
const unsigned long WATER_SEND_EVENT_INTERVAL_MS = (30 * 1000); // 30 seconds

// ========================
// TIMING
// ========================
const unsigned long WATER_INTERVAL_MS = 100;
const unsigned long SCREEN_REFRESH_RATE_MS = 150;

// ========================
// WATER MEASUREMENT
// ========================
const int SLIDING_WINDOW_SIZE = 30;
const int TOUCH_PAD_FILTER_POLLING = 10;

// ========================
// WIFI
const char* WIFI_NAME = SECRET_SSID;
const char* WIFI_PASSWORD = SECRET_PASS;
const char* APP_URL = "https://iot-project-6i3k.onrender.com";

// ========================
// BOTTLE PROPERTIES
// ========================
const int BOTTLE_ML = 500;
int BOTTLE_ID = 1;

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

#endif //IOT_PROJECT_DEFINITIONS_H
