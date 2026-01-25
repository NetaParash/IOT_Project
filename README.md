# Smart Bottle IOT Project

Many students do not drink enough water during the school day, and teachers lack real-time visibility into students’ hydration status. This project features a smart water bottle and a companion app that automatically tracks drinking habits and provides real-time hydration monitoring for students and teachers.

## Key Features

* **Real-time Tracking:** Automatically calculates the amount of water consumed.
* **Smart Stability Detection:** Uses a gyroscope to ensure measurements are only taken when the bottle is stable to prevent errors.
* **Cloud Connectivity:** Syncs hydration data and settings with a backend server via Wi-Fi.
* **Visual Feedback:** An OLED screen displays current status, and an RGB LED provides visual alerts for drinking reminders.
* **Customizable Modes:** Different modes (e.g., Hydration, Sport, Office) for daily goal and alerts, configurable via the device menu or the app. A custom mode is also available via the app.



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
2.  **Sampling:** When the bottle becomes stable (accel/gyro variations drop below a threshold), the system begins sampling the water level, using the logic in `WaterLevelSensor.h`.
3.  **Sliding Window:** It uses a sliding window (buffer) with 20 consecutive water-level readings.
4.  **Calculation:** If all readings in the window are consistent, the system compares the **current stable level** against the **previous stable level**.
    * If `Current < Previous`, the difference is recorded as a **Drink Event**.
    * The total is updated and synced to the cloud via `AppClient`.
Using a sliding window enables us to smooth out noisy measurements and ignore momentary level changes caused by water moving inside the bottle after it has stabilized.


## Code Structure & Class Overview

### Project Structure

```
.
├── Bottle
│   └── Main
│       ├── AppClient.h        # Wi-Fi & Backend communication
│       ├── BottleMode.h       # Data structures for goals/modes
│       ├── ButtonInput.h      # Button interface
│       ├── GyroSensor.h       # Gyro stability check
│       ├── LightNotifier.h    # NeoPixel LED
│       ├── Main.ino           # Main entry point & state machine
│       ├── Parameters.h       # Global constants & configuration
│       ├── Screen.cpp         # OLED display implementation
│       ├── Screen.h           # OLED display interface
│       └── WaterLevelSensor.h # Capacitive sensor & water level measurement logic
├── HW_unit_tests              # Initial hardware testing
├── backend                    # Server-side code
├── frontend                   # Mobile/Web app code
└── README.md
```

### Frontend / Backend Documentation
For details on the mobile/web application, see the [Frontend README](frontend/README.md).

For details on the server-side code, see the [Backend README](backend/README.md).

### Bottle Core Logic

The `Bottle` code is written in C++ (Arduino framework) and consists of the following modules:

* **`Main.ino`**: The entry point. It manages the main loop, coordinates the state machine (Home, Menu, Reset), and integrates all hardware components.
* **`Parameters.h`**: Stores global configuration variables, Wi-Fi settings, and constants (bottle volume, thresholds).

#### Sensors & Inputs
* **`WaterLevelSensor.h`**: Manages the capacitive touch pads. It converts raw capacitance data into a discrete water level between 0 and 8, converted to percentage and Milliliters (mL). It handles the logic for the 8 discrete steps by detecting wet/dry and mid-point thresholds, having two different numeric thresholds for each point, serving as a **"gray area"**. We then process the pads from the **bottom up** and stop scanning at the first dry sensor. This logic filters out false positives from holding the bottle, as water readings require a continuous touch starting from the bottom.
* **`GyroSensor.h`**: Interfaces with the **MPU6050**. It determines if the bottle is currently stable (resting) or moving/tilting to prevent false readings.
* **`ButtonInput.h`**: Handles physical buttons (Next/Select). It includes debouncing logic and edge detection (`wasPressed`) to ensure single-click responsiveness.

#### Connectivity & Data
* **`AppClient.h`**: Handles Wi-Fi connectivity and HTTP REST API communication.
    * `sendEvent()`: Pushes water level and total consumed to the backend.
    * `getSettings()`: Pulls user preferences (Goals, Modes) from the server.
* **`BottleMode.h`**: Defines the data structure for different operating modes (e.g., "Hydration", "Sport"). Holds the logic for daily goals and alert intervals.

#### User Interface (Outputs)
* **`Screen.h` / `Screen.cpp`**: Controls the **OLED display**. It renders the home dashboard, menus, and Wi-Fi status indicator.
* **`LightNotifier.h`**: Controls the **NeoPixel**. It manages visual alerts (blinking blue lights) to remind the user to drink based on the time interval defined in the current mode.

### Libraries & Dependencies
The following libraries were used in this project.
*Note: Please verify the specific installed versions in your environment (Arduino IDE: Tools -> Manage Libraries).*

* **ArduinoJson** by Benoit Blanchon [v7.4.2]
* **Adafruit GFX Library** by Adafruit [v1.12.4]
* **Adafruit SH110X** by Adafruit [v2.1.14] (for the 1.3INCH OLED)
* **Adafruit NeoPixel** by Adafruit [v1.15.2]
* **Adafruit MPU6050** by Adafruit [v2.x.x]
* **Adafruit Unified Sensor** by Adafruit [v1.1.15]
* **WiFi** (Built-in ESP32 library)
* **HTTPClient** (Built-in ESP32 library)
* **WiFiClientSecure** (Built-in ESP32 library)



## Hardware Used in The Project
* **Microcontroller:** ESP32
* **Gyro:** MPU6050 (Accelerometer/Gyroscope)
* **Display:** 1.3INCH OLED
* **LED:** 3 lights NeoPixel strip
* **Inputs:** 2 Buttons
* **Sensing:** Copper tape pads connected to ESP32 Touch pins

### Wires Diagram:
<img width="500" alt="wires_diagram" src="https://github.com/user-attachments/assets/e03c81ab-ae25-4adc-a477-7cdc22a96f95" />

