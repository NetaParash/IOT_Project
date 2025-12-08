#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "Screen.h"   // adjust path if needed

// ------------------------------------------------------
//     I2C Pins for OLED Display (ESP32 example)
// ------------------------------------------------------
#define SDA_PIN 21
#define SCL_PIN 22
// Create screen object using your new ctor
Screen screen(Serial, SDA_PIN, SCL_PIN);

// ------------------------------------------------------
//                       SETUP
// ------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.println("Initializing Screen...");
  screen.setup();

  Serial.println("Displaying test text...");
  screen.print("Hello Bottle!");
}

// ------------------------------------------------------
//                       LOOP
// ------------------------------------------------------
void loop() {
  // Nothing here yet — screen class works independently
}
