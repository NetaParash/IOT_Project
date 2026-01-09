#ifndef GYRO_SENSOR_H
#define GYRO_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

/********************************************
 *               GyroSensor Class
 ********************************************/
class GyroSensor {
private:
    Adafruit_MPU6050 mpu;
    int sdaPin, sclPin, vccPin;

    // Thresholds for stability
    const float GYRO_THRESHOLD = 3.0;      // deg/sec
    const int STABILITY_WINDOW_MS = 10;    // ms
    const float ACCEL_TILT_THRESHOLD = 0.15;   // g
    const float ACCEL_Z_TARGET = 1.0;          // g


public:
    GyroSensor(int sda, int scl, int vcc = -1)
        : sdaPin(sda), sclPin(scl), vccPin(vcc) {}

    bool setup() {
        if (vccPin != -1) {        // optional VCC control
            pinMode(vccPin, OUTPUT);
            digitalWrite(vccPin, HIGH);
            delay(50);
        }

        // Wire.begin(sdaPin, sclPin);

        if (!mpu.begin()) {
            Serial.println("Failed to initialize MPU6050!");
            return false;
        }

        // Configure MPU
        mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
        mpu.setGyroRange(MPU6050_RANGE_250_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

        Serial.println("MPU6050 initialized!");
        return true;
    }

    bool isStable() {
        unsigned long start = millis();

        while (millis() - start < STABILITY_WINDOW_MS) {
            sensors_event_t accel, gyro, temp;
            mpu.getEvent(&accel, &gyro, &temp);

            // ---- Gyro: check no rotation ----
            float gx = gyro.gyro.x * 180 / PI; // rad/s → deg/s
            float gy = gyro.gyro.y * 180 / PI;
            float gz = gyro.gyro.z * 180 / PI;

            if (abs(gx) > GYRO_THRESHOLD ||
                abs(gy) > GYRO_THRESHOLD ||
                abs(gz) > GYRO_THRESHOLD)
            {
                String text_1 = "gx: " + String(gx) + " gy: " + String(gy) + " gz: " + String(gz);
                Serial.println(text_1);
                return false; // rotating
            }

            // ---- Accelerometer: check horizontal ----
            // Convert m/s^2 → g
            float ax = accel.acceleration.x / 9.81;
            float ay = accel.acceleration.y / 9.81;
            float az = accel.acceleration.z / 9.81;

            // Must be flat: X≈0, Y≈0, Z≈1g
            if (abs(ax) > ACCEL_TILT_THRESHOLD ||
                abs(ay) > ACCEL_TILT_THRESHOLD ||
                abs(az - ACCEL_Z_TARGET) > ACCEL_TILT_THRESHOLD)
            {
                String text_2 = "ax: " + String(ax) + " ay: " + String(ay) + " az: " + String(az);
                Serial.println(text_2);
                return false; // tilted
            }
            delay(1);
        }
        return true; // stable AND horizontal
    }
};

#endif
