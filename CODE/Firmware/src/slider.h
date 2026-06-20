#pragma once

#include <array>

#include "MPR121.h"

static constexpr uintptr_t CAP_ADDR = 0x5A;
static constexpr size_t CAP_ZONES_PER_CHIP = 12;
static constexpr size_t CAP_ZONES = 32;
static constexpr size_t CAP_COUNT = (CAP_ZONES + CAP_ZONES_PER_CHIP - 1) / CAP_ZONES_PER_CHIP;

static_assert(CAP_ZONES <= 32, "Can only transmit 32 touch zones over protocol");
static_assert(CAP_ZONES_PER_CHIP % 2 == 0, "Must have an even number of zones per chip");

class chu_slider {
    public:
    chu_slider();

    void setup();

    uint32_t tick();

    private:
    std::array<MPR121, CAP_COUNT> _sensors;
};
