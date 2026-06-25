#include "air.h"

#include <Wire.h>

#include "defs.h"

chu_air::chu_air() {

}

void chu_air::setup() {
#if USE_SERIAL
    Serial.printf("[chu_air] Initializing on %p, %zu boards with %zu sensors each\n", TOF_ADDR, TOF_BOARDS, TOF_COUNT);
#endif

    for (size_t i = 0; i < _sensors.size(); ++i) {
        VL53L0X& sensor = _select_sensor(i);

        sensor.setTimeout(500);

        if (sensor.init()) {
#if USE_SERIAL
            Serial.printf("[chu_air] Started sensor %zu (%i on %p)\n", i, i % TOF_COUNT, TOF_ADDR + (i / TOF_COUNT));
        } else {
            Serial.printf("[chu_air] Failed to start sensor %zu (%i on %p)\n", i, i % TOF_COUNT, TOF_ADDR + (i / TOF_COUNT));
            Serial.printf("          Last status: %i\n", (int)sensor.last_status);
#endif
        }

        sensor.startContinuous();
    }
}

uint8_t chu_air::tick() {
    uint8_t res = 0;

    size_t count_in_range = 0;
    for (size_t i = 0; i < _sensors.size(); ++i) {
        VL53L0X& sensor = _select_sensor(i);

        uint16_t dist = sensor.readRangeContinuousMillimeters();

        if (dist > AIR_OFFSET && dist < (AIR_OFFSET + (AIR_STEP * 6) + (AIR_STEP * 3))) {
            uint8_t height = (dist - AIR_OFFSET + AIR_STEP - 1) / AIR_STEP;
            
            res |= (1 << (height - 1));
        }
    }

    return res;
}

VL53L0X& chu_air::_select_sensor(uint8_t ch) {
    for (uint8_t addr = TOF_ADDR; addr < (TOF_ADDR + TOF_BOARDS); ++addr) {
        uint8_t data = 0;
        if (addr == TOF_ADDR + (ch / TOF_COUNT)) {
            data = 1 << (ch % TOF_COUNT);
        }

        Wire.beginTransmission(addr);
        Wire.write(data);
        Wire.endTransmission();
    }

    return _sensors[ch];
}