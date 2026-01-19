
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
const int SLIDING_WINDOW_SIZE = 20;

// ========================
// WIFI
// ========================
const char* WIFI_NAME = "2Caesars";
const char* WIFI_PASSWORD = "14121998";
const char* APP_URL = "https://iot-project-6i3k.onrender.com";

// ========================
// BOTTLE PROPERTIES
// ========================
const int BOTTLE_ML = 500;

#endif //IOT_PROJECT_DEFINITIONS_H
