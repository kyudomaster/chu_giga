#include "slider.h"

#include <Wire.h>

#include "defs.h"

chu_slider::chu_slider() {
    
}

void chu_slider::setup() {
#if USE_SERIAL
    Serial.printf("[chu_slider] Using %zu zones across %zu sensors, starting at %p\n", CAP_ZONES, CAP_COUNT, CAP_ADDR);
#endif

    for (size_t i = 0; i < CAP_COUNT; ++i) {
        _sensors[i] = MPR121(CAP_ADDR + i);
        
        if (_sensors[i].begin()) {
#if USE_SERIAL
            Serial.printf("[chu_slider] Started sensor %zu at %p\n", i, CAP_ADDR + 1);
        } else {
            Serial.printf("[chu_slider] Failed to start sensor %zu at %p\n", i, CAP_ADDR + 1);
#endif
        }
    }
}

/**
 * Map from:
 *  0  2  4  6  8 10 12 14 16 18 20 22 24 26 28 30
 *  1  3  5  7  9 11 13 15 17 19 21 23 25 27 29 31
 *
 * To:
 * 30 28 26 24 22 20 18 16 14 12 10  8  6  4  2  0
 * 31 29 27 25 23 21 19 17 15 13 11  9  7  5  3  1
 * 
 */

uint32_t chu_slider::tick() {
    uint32_t res = 0;

    size_t remaining = CAP_ZONES;
    for (size_t i = 0; i < _sensors.size(); ++i) {
        uint16_t state = _sensors[i].touched();

        for (size_t j = 0; j < (remaining > CAP_ZONES_PER_CHIP ? CAP_ZONES_PER_CHIP : remaining); j += 2) {
#if USE_SERIAL
            if (state & 1) {
                Serial.printf("%zu is pressed\n", remaining - j);
            }            

            if (state & 0b10) {
                Serial.printf("%zu is pressed\n", remaining - j - 1);
            }
#endif
            res |= ((state & 0b11) << (remaining - 2 - j));
            state >>= 2;
        }

        remaining -= CAP_ZONES_PER_CHIP;
    }

    return res;
}