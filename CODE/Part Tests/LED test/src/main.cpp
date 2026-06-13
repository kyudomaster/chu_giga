#include <Adafruit_NeoPixel.h>

#define DATA_PIN 2
#define NUM_LEDS 1

Adafruit_NeoPixel leds(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;

    if (WheelPos < 85) {
        return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }

    WheelPos -= 170;
    return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup() {
  leds.begin();
  leds.setBrightness(8);
  leds.clear();
  leds.show();
}

void loop() {
  // // Rainbow cycle
  //   for (int i = 0; i < 256; i++) {
  //       leds.setPixelColor(0, Wheel(i));
  //       leds.show();
  //       delay(15);
  //   }
    leds.setPixelColor(0, 255, 0, 0); // Red
    leds.show();
    delay(1000);

    leds.setPixelColor(0, 0, 255, 0); // Green
    leds.show();
    delay(1000);

    leds.setPixelColor(0, 0, 0, 255); // Blue
    leds.show();
    delay(1000);

    leds.setPixelColor(0, 255, 255, 255); // White
    leds.show();
    delay(1000);

    leds.clear();
    leds.show();
    delay(1000);
}