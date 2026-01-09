#include <Arduino.h>
#include "WaterLevelSensor.h"
#include "Screen.h"
#include "GyroSensor.h"
#include "ButtonInput.h"
#include <deque>

// >>> ADDED
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// <<< ADDED

using namespace std;

// ========================
// WIFI + BACKEND CONFIG  (ADDED)
// ========================
const char* WIFI_SSID = "BONITI2";
const char* WIFI_PASS = "25072024";
const char* BACKEND_URL =
    "https://football-site-backend.onrender.com/api/bottle/data";

// send to backend every 5 seconds (safe)
unsigned long lastBackendSend = 0;
const unsigned long BACKEND_INTERVAL_MS = 5000;

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

// ========================
// WIFI CONNECT (ADDED)
// ========================
void connectToWiFi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

// ========================
// SEND DATA TO BACKEND (ADDED)
// ========================
void sendToBackend(int levelPercent, int totalDrankML) {
    if (WiFi.status() != WL_CONNECTED) return;

    WiFiClientSecure client;
    client.setInsecure();   // HTTPS without certificate (PoC)

    HTTPClient http;
    http.begin(client, BACKEND_URL);
    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"water_level_percent\": " + String(levelPercent) + ",";
    payload += "\"total_drank_ml\": " + String(totalDrankML) + ",";
    payload += "\"timestamp\": " + String(millis());
    payload += "}";

    int code = http.POST(payload);
    Serial.print("Backend POST -> ");
    Serial.println(code);

    http.end();
}


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