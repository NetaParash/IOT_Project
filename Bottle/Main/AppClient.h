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
              const char* backendDomain,
              int bottleId)
        : _ssid(ssid),
          _password(password),
          _backend(backendDomain),
          _bottleId(bottleId) {}

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

        if (!connectWiFi()) return false;

        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient https;

        String url = _backend + "/api/bottle/" + String(_bottleId) + "/settings";

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
        Serial.println(payload);

        int code = https.POST(payload);
        Serial.print("[HTTP] status: ");
        Serial.println(code);

        if (code > 0) Serial.println(https.getString());

        https.end();
        return code == 200;
    }

    /* ========================
       Get settings
       ======================== */
    std::vector<String> getSettings() {
        Serial.println("[HTTP] getSettings()");
        std::vector<String> result;

        if (!connectWiFi()) return result;

        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient https;

        String url = _backend + "/api/bottle/" + String(_bottleId) + "/settings";

        Serial.print("[HTTP] GET ");
        Serial.println(url);

        https.begin(client, url);
        int code = https.GET();

        Serial.print("[HTTP] status: ");
        Serial.println(code);

        if (code == 200) {
            String body = https.getString();
            Serial.println(body);

            StaticJsonDocument<200> doc;
            if (!deserializeJson(doc, body)) {
                result.push_back(doc["mode"].as<String>());
                result.push_back(String(doc["goal"].as<int>()));
                result.push_back(String(doc["alerts_every"].as<int>()));
            }
        }

        https.end();
        return result;
    }

    /* ========================
       Get last total drank
       ======================== */
    int getLastTotalDrank() {
        Serial.println("[HTTP] getLastTotalDrank()");
        int result = 0;

        if (!connectWiFi()) return 0;

        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient https;

        String url = _backend + "/api/app/" + String(_bottleId) + "/total-drank-today";

        Serial.print("[HTTP] GET ");
        Serial.println(url);

        https.begin(client, url);
        int code = https.GET();

        Serial.print("[HTTP] status: ");
        Serial.println(code);

        if (code == 200) {
            String body = https.getString();
            Serial.println(body);

            StaticJsonDocument<200> doc;
            if (!deserializeJson(doc, body)) {
                result = doc["total_drank_today_ml"].as<int>();
            }
        }

        https.end();
        return result;
    }

    /* ========================
       Send drink event
       ======================== */
    bool sendEvent(int amountDrankMl, int waterLevelMl) {
        Serial.println("[HTTP] sendEvent()");

        if (!connectWiFi()) return false;

        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient https;

        String url = _backend + "/api/bottle/" + String(_bottleId) + "/events";

        Serial.print("[HTTP] POST ");
        Serial.println(url);

        https.begin(client, url);
        https.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> doc;
        doc["amount_drank_ml"] = amountDrankMl;
        doc["water_level_ml"] = waterLevelMl;

        String payload;
        serializeJson(doc, payload);
        Serial.println(payload);

        int code = https.POST(payload);

        Serial.print("[HTTP] status: ");
        Serial.println(code);
        if (code > 0) Serial.println(https.getString());

        https.end();
        return code == 200;
    }

    /* ========================
       Clear event data
       ======================== */
    bool clearEventData() {
        Serial.println("[HTTP] clearEventData()");

        if (!connectWiFi()) return false;

        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient https;

        String url = _backend + "/api/app/" + String(_bottleId) + "/clear-event-data";

        Serial.print("[HTTP] POST ");
        Serial.println(url);

        https.begin(client, url);
        https.addHeader("Content-Type", "application/json");

        int code = https.POST("{}");

        Serial.print("[HTTP] status: ");
        Serial.println(code);
        if (code > 0) Serial.println(https.getString());

        https.end();
        return code == 200;
    }

private:
    const char* _ssid;
    const char* _password;
    String _backend;
    int _bottleId;
};

#endif
