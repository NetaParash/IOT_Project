#ifndef LIGHT_NOTIFIER_H
#define LIGHT_NOTIFIER_H

#include <Adafruit_NeoPixel.h>

class LightNotifier {
  private:
    Adafruit_NeoPixel _strip; 
    int _pin;
    int _numPixels;

    void setAllColors(uint32_t color) {
      for(int i = 0; i < _numPixels; i++) {
        _strip.setPixelColor(i, color);
      }
      _strip.show();
    }

  public:
    LightNotifier(int pin, int numPixels) 
      : _strip(numPixels, pin, NEO_GRB + NEO_KHZ800),
        _pin(pin), 
        _numPixels(numPixels) {}

    void setup() {
      _strip.begin();
      _strip.show(); 
      _strip.setBrightness(200);
    }

    void blinkNotification() {
      unsigned long startTime = millis();
      unsigned long duration = 10000;
      
      uint32_t blue = _strip.Color(0, 0, 255);
      uint32_t off = _strip.Color(0, 0, 0);

      while (millis() - startTime < duration) {
        setAllColors(blue);
        delay(500);

        if (millis() - startTime >= duration) break;

        setAllColors(off);
        delay(500); 
      }
      setAllColors(off);
    }
};

#endif