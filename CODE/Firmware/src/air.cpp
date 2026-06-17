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
    uint16_t min_distance = 0xFFFF;

#if USE_SERIAL
    static int tick_n = 0;
    const bool should_print = tick_n % 1000 == 0;
    
    if (should_print)
        Serial.printf("[chu_air] Distances:\n");
#endif

    size_t count_in_range = 0;
    for (size_t i = 0; i < _sensors.size(); ++i) {
        VL53L0X& sensor = _select_sensor(i);

        uint16_t dist = sensor.readRangeContinuousMillimeters();

#if USE_SERIAL
        if (should_print) {
            if (sensor.timeoutOccurred()) {
                Serial.printf("    [%02d] -> Timed out\n", i);
            } else if (should_print) {
                Serial.printf("    [%02d] -> %hu mm\n", i, dist);
            }
        } else {
            Serial.printf("[chu_air] Channel %i timed out after %hu\n", i, sensor.getTimeout());
        }
        
#endif

        if (dist > AIR_OFFSET && dist < (AIR_OFFSET + (AIR_STEP * 6))) {
            ++count_in_range;

            if (dist < min_distance) {
                min_distance = dist;
            }
        }
    }

#if USE_SERIAL
    if (should_print)
        Serial.printf("    Min distance of %hu mm over %zu sensors\n", min_distance, count_in_range);
    
        ++tick_n;
#endif

    if (count_in_range < 2 || min_distance == 0xFFFF) {
        return 0;
    }

    return (min_distance - AIR_OFFSET + AIR_STEP - 1) / AIR_STEP;
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