#pragma once

#include <array>

#include "VL53L0X.h"

static constexpr uintptr_t TOF_ADDR = 0x70;
static constexpr size_t TOF_BOARDS = 2;
static constexpr size_t TOF_COUNT = 6;
static constexpr size_t TOF_TOTAL = TOF_BOARDS * TOF_COUNT;

static constexpr uint16_t AIR_OFFSET = 172;
static constexpr uint16_t AIR_STEP = 32;

class chu_air {
    public:
    chu_air();

    void setup();

    uint8_t tick();

    private:
    std::array<VL53L0X, TOF_TOTAL> _sensors;

    VL53L0X& _select_sensor(uint8_t ch);
};
