#include "air.h"

#include <Wire.h>

#define AIR_OFFSET 172
#define AIR_STEP 34

chu_air::chu_air(uint8_t addr)
    : _addr { addr } {

}

void chu_air::setup() {
    Serial.printf("[chu_air] Initializing on %p\n", _addr);

    for (size_t i = 0; i < _sensors.size(); ++i) {
        VL53L0X& sensor = _select_sensor(i);

        sensor.setTimeout(500);

        if (sensor.init()) {
            Serial.printf("[chu_air] Started sensor %zu\n", i);
        } else {
            Serial.printf("[chu_air] Failed to start sensor %zu\n", i);
        }

        sensor.startContinuous();
    }
}

uint8_t chu_air::tick() {
    uint16_t min_distance = 0xFFFF;

    for (size_t i = 0; i < 6; ++i) {
        VL53L0X& sensor = _select_sensor(i);

        uint16_t dist = sensor.readRangeContinuousMillimeters();

        if (dist > AIR_OFFSET && dist < (AIR_OFFSET + (AIR_STEP * 6)) && dist < min_distance) {
            min_distance = dist;
        }
    }

    if (min_distance == 0xFFFF) {
        return 0;
    }

    return (min_distance - AIR_OFFSET + AIR_STEP - 1) / AIR_STEP;
}

VL53L0X& chu_air::_select_sensor(uint8_t ch) {
    Wire.beginTransmission(_addr);
    Wire.write(1 << ch);
    Wire.endTransmission();

    return _sensors[ch];
}