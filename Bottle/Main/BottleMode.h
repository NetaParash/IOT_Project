#ifndef BOTTLE_MODE_H
#define BOTTLE_MODE_H

#include <Arduino.h>
#include <vector>

#define MODE_COUNT 4

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

static const BottleMode modes[MODE_COUNT] = {
        BottleMode("hydration", 2500, 60),
        BottleMode("sport",     3500, 30),
        BottleMode("office",    2000, 90),
        BottleMode("night",     500,  0)
};

// Initialize with mode HYDRATION (index 0)
BottleMode currentMode(modes[0].name, modes[0].dailyGoal, modes[0].alertEveryMinutes);


#endif