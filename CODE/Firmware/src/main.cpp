#include <Wire.h>

#include "usb.h"
#include "air.h"

#define REPORT_RATE 1000
#define REPORT_RATE_US (1'000'000 / REPORT_RATE)

#define TCAADDR 0x71 // or 0x70 for the other ToF board.

static chu_air air { TCAADDR };
static chu_usb usb_hid { air };

void setup() {
    Wire.setSDA(16);
    Wire.setSCL(17);
    Wire.begin();

    // Serial.begin(115200);
    // while (!Serial) { }

    //Serial.println("Setting up USB HID");
    usb_hid.setup();

    // Emulate same device RedBoard expects
    // USBDevice.setID(0x0f0d, 0x0092);

    while (!USBDevice.mounted() && !usb_hid.ready()) {
        delay(1);
    }
}

void loop() {
    uint64_t next_tick = time_us_64() + REPORT_RATE_US;

    for (;;) {
        tud_task();

        usb_hid.tick();

        sleep_until(next_tick);
        next_tick += REPORT_RATE_US;
    }
}
