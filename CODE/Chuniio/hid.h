#pragma once

#include <Windows.h>

#include <cstdint>
#include <atomic>
#include <thread>

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

    uint32_t slider() const;
    uint8_t air() const;

    private:
    std::atomic_bool _initialized = false;
    HANDLE _file = INVALID_HANDLE_VALUE;

    std::atomic_bool _stop_thread = false;
    std::thread _usb_thread;

    std::atomic<report_data> _report;
};
