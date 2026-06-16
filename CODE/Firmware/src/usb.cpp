#include "usb.h"

#include "air.h"

chu_usb::chu_usb(chu_air& air)
    : _air { air } {
    _hid.setPollInterval(1);
    _hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
    _hid.setStringDescriptor("chu_gamepad");
}

void chu_usb::setup() {
    Serial.println("[chu_usb] setup");

    if (!_hid.begin()) {
        Serial.println("[chu_usb] Failed to start HID device");
    } else {
        Serial.println("[chu_usb] Started HID device");
    }

    _air.setup();
}

bool chu_usb::ready() {
    return _hid.ready();
}

bool chu_usb::tick() {
    _update_buttons();
    _report.air = _air.tick();

    return _send_report();
}

void chu_usb::_update_buttons() {
    _report.buttons += 1;
}

bool chu_usb::_send_report() {
    return _hid.sendReport(0, &_report, sizeof(_report));
}
