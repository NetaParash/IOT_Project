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
    const float GYRO_THRESHOLD = 5.0;          // deg/sec
    const int STABILITY_WINDOW_MS = 10;        // ms
    const float ACCEL_TILT_THRESHOLD = 0.15;   // g

    // ---- FIXED GYRO CALIBRATION (YOUR VALUES) ----
const float baseGx = -1.92;
const float baseGy = -0.68;
const float baseGz = -0.05;


    // ---- Accelerometer baseline (auto) ----
    float baseAx = 0;
    float baseAy = 0;
    float baseAz = 1.0;

public:
    GyroSensor(int sda, int scl, int vcc = -1)
        : sdaPin(sda), sclPin(scl), vccPin(vcc) {}

    bool setup() {
        if (vccPin != -1) {
            pinMode(vccPin, OUTPUT);
            digitalWrite(vccPin, HIGH);
            delay(50);
        }

        if (!mpu.begin()) {
            Serial.println("Failed to initialize MPU6050!");
            return false;
        }

        mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
        mpu.setGyroRange(MPU6050_RANGE_250_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

        // ---- Accelerometer baseline calibration ----
        sensors_event_t a, g, t;
        mpu.getEvent(&a, &g, &t);

        baseAx = a.acceleration.x / 9.81;
        baseAy = a.acceleration.y / 9.81;
        baseAz = a.acceleration.z / 9.81;

        Serial.println("MPU6050 initialized");
        Serial.println("Gyro baseline locked to:");
        Serial.println(String("gx=") + baseGx +
                       " gy=" + baseGy +
                       " gz=" + baseGz);
        
        return true;
    }

    bool isStable() {
        unsigned long start = millis();

        while (millis() - start < STABILITY_WINDOW_MS) {

            sensors_event_t accel, gyro, temp;
            mpu.getEvent(&accel, &gyro, &temp);

            // ---- Gyro: compare RELATIVE to baseline ----
            float gx = gyro.gyro.x * 180 / PI;
            float gy = gyro.gyro.y * 180 / PI;
            float gz = gyro.gyro.z * 180 / PI;

            if (abs(gx - baseGx) > GYRO_THRESHOLD ||
                abs(gy - baseGy) > GYRO_THRESHOLD ||
                abs(gz - baseGz) > GYRO_THRESHOLD)
            {
                Serial.println(
                    "gx: " + String(gx) +
                    " gy: " + String(gy) +
                    " gz: " + String(gz)
                );
                return false;
            }

            // ---- Accelerometer: baseline-relative tilt ----
            float ax = accel.acceleration.x / 9.81;
            float ay = accel.acceleration.y / 9.81;
            float az = accel.acceleration.z / 9.81;

            if (abs(ax - baseAx) > ACCEL_TILT_THRESHOLD ||
                abs(ay - baseAy) > ACCEL_TILT_THRESHOLD ||
                abs(az - baseAz) > ACCEL_TILT_THRESHOLD)
            {
                return false;
            }

            delay(1);
        }

        return true;
    }
};

#endif
