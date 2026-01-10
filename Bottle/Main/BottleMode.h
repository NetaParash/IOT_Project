#ifndef BOTTLE_MODE_H
#define BOTTLE_MODE_H

#include <Arduino.h>
#include <vector>

class BottleMode {
public:
    String name;
    int dailyGoal;
    int alertEveryMinutes;

    BottleMode() = delete;
    BottleMode(String n, int g, int a) : name(n), dailyGoal(g), alertEveryMinutes(a) {};

    unsigned long getAlertIntervalMs() const {
        // Convert the alert interval from minutes to milliseconds
        return (unsigned long)alertEveryMinutes * 60 * 1000;
    }
};

#endif