
#ifndef IOT_PROJECT_DEFINITIONS_H
#define IOT_PROJECT_DEFINITIONS_H

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
// ========================
const char* WIFI_NAME = "OrZ iPhone";
const char* WIFI_PASSWORD = "g0iibm9ik7ry";
const char* APP_URL = "https://iot-project-6i3k.onrender.com";

// ========================
// BOTTLE PROPERTIES
// ========================
const int BOTTLE_ML = 500;
int BOTTLE_ID = 1;

#endif //IOT_PROJECT_DEFINITIONS_H
