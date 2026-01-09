#ifndef APP_CLIENT_TEST_H
#define APP_CLIENT_TEST_H

#include "AppClient.h"

void testAppClientDrainBottle() {
    Serial.println();
    Serial.println("====== AppClient DRAIN TEST ======");

    AppClient client(
        "OrZ iPhone",
        "g0iibm9ik7ry",
        "https://iot-project-6i3k.onrender.com"
    );

    if (!client.connectWiFi()) {
        Serial.println("[TEST] WiFi unavailable, aborting test");
        return;
    }
    Serial.println("[TEST] Clearing backend event data");
    client.clearEventData();

     delay(1000);   
     Serial.println("[TEST] Starting bottle drain simulation");

    int waterLevel = 500;        // ml
    int totalDrank = 0;
    const int step = 50;         // ml per drink
    const int delayMs = 2000;    // 2 seconds

    while (waterLevel > 0) {
        totalDrank += step;
        waterLevel -= step;

        if (waterLevel < 0) {
            waterLevel = 0;
        }

        Serial.print("[TEST] sendEvent → drank=");
        Serial.print(totalDrank);
        Serial.print(" ml, water=");
        Serial.print(waterLevel);
        Serial.println(" ml");

        client.sendEvent(totalDrank, waterLevel);

        delay(delayMs);
    }

    Serial.println("[TEST] Bottle is empty (0 ml)");
    Serial.println("====== TEST END ======");
    Serial.println();
}

#endif
