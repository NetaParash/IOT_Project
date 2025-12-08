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
    const int STABILITY_WINDOW_MS = 10;    // time window

public:
    GyroSensor(int sda, int scl, int vcc = -1)
        : sdaPin(sda), sclPin(scl), vccPin(vcc) {}

    bool setup() {
        if (vccPin != -1) {        // optional VCC control
            pinMode(vccPin, OUTPUT);
            digitalWrite(vccPin, HIGH);
            delay(50);
        }

        Wire.begin(sdaPin, sclPin);

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

            float gx = gyro.gyro.x * 180 / PI; // rad/s → deg/s
            float gy = gyro.gyro.y * 180 / PI;
            float gz = gyro.gyro.z * 180 / PI;

            // If any exceeds threshold → not stable
            if (abs(gx) > GYRO_THRESHOLD ||
                abs(gy) > GYRO_THRESHOLD ||
                abs(gz) > GYRO_THRESHOLD) 
            {
                return false;
            }

            delay(1);  // reduces noise
        }

        return true; // fully stable during window
    }
};
