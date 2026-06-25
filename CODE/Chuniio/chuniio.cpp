#include "chuniio.h"

#include "hid.h"

#include <string>
#include <format>
#include <thread>
#include <array>

void message(const std::wstring& msg) {
    MessageBox(
        NULL,
        msg.c_str(),
        L"OK",
        MB_ICONEXCLAMATION | MB_OK
    );
}

uint16_t chuni_io_get_api_version() {
    return 0x0102;
}

static chu_hid hid;

HRESULT chuni_io_jvs_init() {
    if (!hid.init()) {
        return S_FALSE;
    }

    return S_FALSE;
}

void chuni_io_jvs_poll(uint8_t* opbtn, uint8_t* beams) {
    if (GetAsyncKeyState(VK_F1) < 0) {
        *opbtn |= 0b01; /* Test */
    }

    if (GetAsyncKeyState(VK_F2) < 0) {
        *opbtn |= 0b10; /* Service */
    }

    *beams = hid.air();
}

static bool pressed = false;
static uint16_t coins = 0;

void chuni_io_jvs_read_coin_counter(uint16_t* out) {
    if (GetAsyncKeyState(VK_F3)) {
        if (!pressed) {
            pressed = true;
            coins += 1;
        }
    } else {
        /* Released */
        pressed = false;
    }

    *out = coins;
}

HRESULT chuni_io_slider_init() {
    return S_OK;
}

static std::atomic_bool should_stop = false;
static std::thread poll_thread;

void chuni_io_slider_start(chuni_io_slider_callback_t callback) {
    should_stop = false;
    poll_thread = std::thread([callback] {
        std::array<uint8_t, 32> data{};

        while (!should_stop) {
            uint32_t slider = hid.slider();

            for (size_t i = 0; i < data.size(); ++i) {
                if (slider & (uint32_t{ 1 } << i)) {
                    data[i] = 128;
                } else {
                    data[i] = 0;
                }
            }

            callback(data.data());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
}

void chuni_io_slider_stop() {
    should_stop = true;
    poll_thread.join();
}

void chuni_io_slider_set_leds(const uint8_t* rgb) {

}

HRESULT chuni_io_led_init() {
    return S_OK;
}

void chuni_io_led_set_colors(uint8_t board, uint8_t* rgb) {

}
