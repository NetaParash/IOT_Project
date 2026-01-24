# Smart Bottle IOT Project

Many students do not drink enough water during the school day, and teachers lack real-time visibility into students’ hydration status. This project features a smart water bottle and a companion app that automatically tracks drinking habits and provides real-time hydration monitoring for students and teachers.

## Key Features

* [cite_start]**Real-time Tracking:** Automatically calculates the amount of water consumed.
* [cite_start]**Smart Stability Detection:** Uses a gyroscope to ensure measurements are only taken when the bottle is stable to prevent errors.
* [cite_start]**Cloud Connectivity:** Syncs hydration data and settings with a backend server via Wi-Fi.
* [cite_start]**Visual Feedback:** An OLED screen displays current status, and an RGB LED provides visual alerts for drinking reminders.
* [cite_start]**Customizable Modes:** Different modes (e.g., Hydration, Sport, Office) for daily goal and alerts, configurable via the device menu or the app. A custom mode is also available via the app.

---

## How It Works

### 1. Measuring Water Level (Capacitive Sensing)
The bottle uses **Capacitive Touch Sensing** to measure water volume non-intrusively.

* **Hardware:** Four conductive touch pads are arranged vertically along the bottle.
* **Logic:** As water fills the bottle, it changes the capacitance of the pads. The `WaterLevelSensor` reads these raw values using specific thresholds for "Wet" and "Dry" states to prevent flickering.
* **Resolution:** The system provides **8 discrete steps** of resolution. We utilize 4 physical touch pads, but the code analyzes the raw capacitance to detect if water covers just the bottom half or the top half of each specific pad. *4 Pads × 2 Zones (Bottom/Top) = 8 Detectable Levels*.
* **Smoothing:** To avoid "jumpy" readings when the water is exactly at a threshold point, we apply a dual-threshold approach. We define a "gray area" where the requirement to enter a higher water level is different from the requirement to drop back down. This prevents the system from bouncing between levels when the raw reading is on the edge, which would otherwise have been falsely detected as drinking events.

### 2. Detecting a "Drink"
The system does not measure water flow while you are tilting the bottle. The logic is as follows:

1.  **Stability Check:** The `GyroSensor` (MPU6050) constantly monitors the bottle's movement.
2.  **Sampling:** When the bottle becomes stable (accel/gyro variations drop below a threshold), the system begins sampling the water level.
3.  **Sliding Window:** It uses a sliding window (buffer) with 20 consecutive water-level readings.
4.  **Calculation:** If all readings in the window are consistent, the system compares the **current stable level** against the **previous stable level**.
    * If `Current < Previous`, the difference is recorded as a **Drink Event**.
    * The total is updated and synced to the cloud via `AppClient`.
Using a sliding window enables us to smooth out noisy measurements and ignore momentary level changes caused by water moving inside the bottle after it has stabilized.
---

## Code Structure & Class Overview

The project is written in C++ (Arduino framework) and consists of the following modules:

### Core Logic
* **`Main.ino`**: The entry point. It manages the main loop, coordinates the state machine (Home, Menu, Reset), and integrates all hardware components.
* **`Parameters.h`**: Stores global configuration variables, pin definitions, Wi-Fi settings, and constants (bottle volume, thresholds).

### Sensors & Inputs
* **`WaterLevelSensor.h`**: Manages the capacitive touch pads. It converts raw capacitance data into a discrete water level between 0 and 8, converted to percentage and Milliliters (mL). It handles the logic for the 8 discrete steps by detecting wet/dry and mid-point thresholds.
* **`GyroSensor.h`**: Interfaces with the **MPU6050**. It determines if the bottle is currently stable (resting) or moving/tilting to prevent water sloshing from creating false readings.
* **`ButtonInput.h`**: Handles physical button interactions (Next/Select). It includes debouncing logic and edge detection (`wasPressed`) to ensure single-click responsiveness.

### Connectivity & Data
* **`AppClient.h`**: Handles Wi-Fi connectivity and HTTP REST API communication.
    * `sendEvent()`: Pushes water level and total consumed to the backend.
    * `getSettings()`: Pulls user preferences (Goals, Modes) from the server.
* **`BottleMode.h`**: Defines the data structure for different operating modes (e.g., "Hydration", "Sport"). Holds the logic for daily goals and alert intervals.

### User Interface (Outputs)
* **`Screen.h` / `Screen.cpp`**: Controls the **OLED display**. It renders the home dashboard, menus, and Wi-Fi status indicator.
* **`LightNotifier.h`**: Controls the **NeoPixel**. It manages visual alerts (blinking blue lights) to remind the user to drink based on the time interval defined in the current mode.

---

## Hardware Used in The Project
* **Microcontroller:** ESP32
* **IMU:** MPU6050 (Accelerometer/Gyroscope)
* **Display:** 1.3INCH OLED
* **LED:** 3 lights NeoPixel strip
* **Inputs:** 2 Buttons
* **Sensing:** Copper tape pads connected to ESP32 Touch pins
