#pragma once

#include <Windows.h>

#include <cstdint>

#pragma pack(push, 1)
struct report_data {
    uint8_t id;
	uint8_t air;
	uint32_t slider;
	uint8_t buttons;
};
#pragma pack(pop)

class chu_hid {
    public:
    ~chu_hid();

    bool init();

    bool read_report(report_data& res);

    private:
    HANDLE _file = INVALID_HANDLE_VALUE;
};
