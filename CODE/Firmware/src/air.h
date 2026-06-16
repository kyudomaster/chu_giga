#pragma once

#include <array>

#include "VL53L0X.h"

class chu_air {
    public:
    chu_air(uint8_t addr);

    void setup();

    uint8_t tick();

    private:
    const uint8_t _addr;

    std::array<VL53L0X, 6> _sensors;

    VL53L0X& _select_sensor(uint8_t ch);
};
