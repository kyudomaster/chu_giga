#pragma once

#include <Adafruit_TinyUSB.h>

#define ATTRIBUTE_PACKED  __attribute__((packed, aligned(1)))

struct report_data {
    uint8_t air;
    uint32_t slider;
    uint8_t buttons;
} ATTRIBUTE_PACKED;

constexpr uint8_t desc_hid_report[] {
    HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     ),
    HID_USAGE      ( HID_USAGE_DESKTOP_JOYSTICK ),
    HID_COLLECTION ( HID_COLLECTION_APPLICATION ),
        /* Report ID if any */
        HID_USAGE_PAGE   ( HID_USAGE_PAGE_DESKTOP                 ),
        HID_USAGE        ( HID_USAGE_DESKTOP_Z                    ),
        HID_LOGICAL_MIN  ( 0x00                                   ),
        HID_LOGICAL_MAX  ( 0x06                                   ),
        HID_REPORT_COUNT ( 1                                      ),
        HID_REPORT_SIZE  ( 8                                      ),
        HID_INPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
        /* 32 bit Button Map */ 
        HID_USAGE_PAGE   ( HID_USAGE_PAGE_BUTTON                  ),
        HID_USAGE_MIN    ( 1                                      ),
        HID_USAGE_MAX    ( 32                                     ),
        HID_LOGICAL_MIN  ( 0                                      ),
        HID_LOGICAL_MAX  ( 1                                      ),
        HID_REPORT_COUNT ( 32                                     ),
        HID_REPORT_SIZE  ( 1                                      ),
        HID_INPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
        HID_USAGE_PAGE   ( HID_USAGE_PAGE_BUTTON                  ),
        HID_USAGE_MIN    ( 1                                      ),
        HID_USAGE_MAX    ( 8                                      ),
        HID_LOGICAL_MIN  ( 0                                      ),
        HID_LOGICAL_MAX  ( 1                                      ),
        HID_REPORT_COUNT ( 8                                      ),
        HID_REPORT_SIZE  ( 1                                      ),
        HID_INPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
    HID_COLLECTION_END
};

class chu_air;

class chu_usb {
    public:
    chu_usb(chu_air& air);

    void setup();

    bool ready();

    bool tick();

    private:
    chu_air& _air;
    report_data _report{};
    Adafruit_USBD_HID _hid;

    void _update_buttons();
    bool _send_report();
};
