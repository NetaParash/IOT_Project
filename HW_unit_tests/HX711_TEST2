/*
  Based on Rui Santos – RandomNerdTutorials HX711 example.
  Original project: https://RandomNerdTutorials.com/esp32-load-cell-hx711/
*/

#include <Arduino.h>
#include "soc/rtc.h"
#include "HX711.h"

// HX711 wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN  = 4;

HX711 scale;

void setup() {
  Serial.begin(115200);

  // Optional: set ESP32 clock to improve ADC stability
  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Tare once at start (remove all weight)
  Serial.println("Taring...");
  scale.set_scale();      
  scale.tare();
  Serial.println("Ready.");
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.get_units(5);  // average of 5 samples
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(100);  // small delay to reduce noise (100 ms)
}
