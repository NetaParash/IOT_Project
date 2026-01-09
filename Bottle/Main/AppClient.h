#ifndef APP_CLIENT_H
#define APP_CLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <vector>

class AppClient {
public:
    /* ========================
       Constructor
       ======================== */
    AppClient(const char* ssid,
              const char* password,
              const char* backendDomain)
        : _ssid(ssid),
          _password(password),
          _backend(backendDomain) {}

    /* ========================
       WiFi connection
       ======================== */
    bool connectWiFi(unsigned long timeoutMs = 5000) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("[WiFi] already connected");
            return true;
        }

        Serial.print("[WiFi] connecting to ");
        Serial.println(_ssid);

        WiFi.begin(_ssid, _password);

        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED) {
            if (millis() - start > timeoutMs) {
                Serial.println("[WiFi] connection timeout");
                return false;
            }
            Serial.print(".");
            delay(300);
        }

        Serial.println();
        Serial.println("[WiFi] connected");
        Serial.print("[WiFi] IP: ");
        Serial.println(WiFi.localIP());
        return true;
    }

    /* ========================
       Send settings
       ======================== */
    bool sendSettings(const String& mode,
                      int goal,
                      int alertsEvery) {

        Serial.println("[HTTP] sendSettings()");

        if (!connectWiFi()) {
            Serial.println("[HTTP] WiFi unavailable, skipping sendSettings");
            return false;
        }

        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        String url = _backend + "/api/bottle/settings";

        Serial.print("[HTTP] POST ");
        Serial.println(url);

        https.begin(client, url);
        https.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> doc;
        doc["mode"] = mode;
        doc["goal"] = goal;
        doc["alerts_every"] = alertsEvery;

        String payload;
        serializeJson(doc, payload);

        Serial.print("[HTTP] payload: ");
        Serial.println(payload);

        int code = https.POST(payload);

        Serial.print("[HTTP] status: ");
        Serial.println(code);

        if (code > 0) {
            Serial.print("[HTTP] response: ");
            Serial.println(https.getString());
        }

        https.end();
        return code == 200;
    }

    /* ========================
       Get settings
       Returns empty vector on failure
       Order: [mode, goal, alerts_every]
       ======================== */
    std::vector<String> getSettings() {
        Serial.println("[HTTP] getSettings()");
        std::vector<String> result;

        if (!connectWiFi()) {
            Serial.println("[HTTP] WiFi unavailable, skipping getSettings");
            return result;
        }

        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        String url = _backend + "/api/bottle/settings";

        Serial.print("[HTTP] GET ");
        Serial.println(url);

        https.begin(client, url);
        int code = https.GET();

        Serial.print("[HTTP] status: ");
        Serial.println(code);

        if (code == 200) {
            String body = https.getString();
            Serial.print("[HTTP] response: ");
            Serial.println(body);

            StaticJsonDocument<200> doc;
            DeserializationError err = deserializeJson(doc, body);

            if (!err) {
                result.push_back(doc["mode"].as<String>());
                result.push_back(String(doc["goal"].as<int>()));
                result.push_back(String(doc["alerts_every"].as<int>()));
            } else {
                Serial.print("[JSON] parse error: ");
                Serial.println(err.c_str());
            }
        }

        https.end();
        return result;
    }

    /* ========================
       Send drink event
       ======================== */
    bool sendEvent(int amountDrankMl,
                   int waterLevelMl) {

        Serial.println("[HTTP] sendEvent()");

        if (!connectWiFi()) {
            Serial.println("[HTTP] WiFi unavailable, skipping sendEvent");
            return false;
        }

        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        String url = _backend + "/api/bottle/events";

        Serial.print("[HTTP] POST ");
        Serial.println(url);

        https.begin(client, url);
        https.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> doc;
        doc["amount_drank_ml"] = amountDrankMl;
        doc["water_level_ml"] = waterLevelMl;

        String payload;
        serializeJson(doc, payload);

        Serial.print("[HTTP] payload: ");
        Serial.println(payload);

        int code = https.POST(payload);

        Serial.print("[HTTP] status: ");
        Serial.println(code);

        if (code > 0) {
            Serial.print("[HTTP] response: ");
            Serial.println(https.getString());
        }

        https.end();
        return code == 200;
    }
    /* ========================
   Clear event data (TEST / RESET)
   ======================== */
bool clearEventData() {
    Serial.println("[HTTP] clearEventData()");

    if (!connectWiFi()) {
        Serial.println("[HTTP] WiFi unavailable, skipping clearEventData");
        return false;
    }

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    String url = _backend + "/api/app/clear-event-data";

    Serial.print("[HTTP] POST ");
    Serial.println(url);

    https.begin(client, url);
    https.addHeader("Content-Type", "application/json");

    // No payload needed, but POST requires something
    int code = https.POST("{}");

    Serial.print("[HTTP] status: ");
    Serial.println(code);

    if (code > 0) {
        Serial.print("[HTTP] response: ");
        Serial.println(https.getString());
    }

    https.end();
    return code == 200;
}


private:
    const char* _ssid;
    const char* _password;
    String _backend;
};

#endif
