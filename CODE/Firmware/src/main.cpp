#include <Wire.h>

#include <Adafruit_TinyUSB.h>

/*void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    Wire.setSDA(16);
    Wire.setSCL(17);
    Wire.begin();
}*/

#define ATTRIBUTE_PACKED  __attribute__((packed, aligned(1)))

typedef struct ATTRIBUTE_PACKED {
    uint8_t axes[3];
    uint32_t buttons;
    uint8_t extra_buttons;
} HID_GamepadReport_Data_t;

#define MY_REPORT_DESC_FUCK_YOU(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_JOYSTICK  )                 ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_X                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Z                    ) ,\
    HID_LOGICAL_MIN    ( 0x00                                   ) ,\
    HID_LOGICAL_MAX    ( 0xff                                   ) ,\
    HID_REPORT_COUNT   ( 3                                      ) ,\
    HID_REPORT_SIZE    ( 8                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* 32 bit Button Map */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
    HID_USAGE_MIN      ( 1                                      ) ,\
    HID_USAGE_MAX      ( 32                                     ) ,\
    HID_LOGICAL_MIN    ( 0                                      ) ,\
    HID_LOGICAL_MAX    ( 1                                      ) ,\
    HID_REPORT_COUNT   ( 32                                     ) ,\
    HID_REPORT_SIZE    ( 1                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
    HID_USAGE_MIN      ( 1                                      ) ,\
    HID_USAGE_MAX      ( 8                                     ) ,\
    HID_LOGICAL_MIN    ( 0                                      ) ,\
    HID_LOGICAL_MAX    ( 1                                      ) ,\
    HID_REPORT_COUNT   ( 8                                     ) ,\
    HID_REPORT_SIZE    ( 1                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END \

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] =
{
    MY_REPORT_DESC_FUCK_YOU(HID_REPORT_ID(1))
};


class TUGamepad {
  public:
    inline TUGamepad(void);

    inline void begin(void);
    inline void end(void);
    inline void loop(void);
    inline void write(void);
    inline void write(void *report);
    inline void press(uint8_t b);
    inline void release(uint8_t b);
    inline void releaseAll(void);

    inline void buttons(uint16_t b);
    inline void leftXAxis(int8_t a);
    inline void leftYAxis(int8_t a);
    inline void rightXAxis(int8_t a);
    inline void rightYAxis(int8_t a);
    inline bool ready(void) { return this->usb_hid.ready(); };

    // Sending is public for advanced users.
    inline bool SendReport(void* data, size_t length) {
        return this->usb_hid.sendReport(1, data, (uint8_t)length);
    };

    HID_GamepadReport_Data_t _report;
    uint32_t startMillis;
    Adafruit_USBD_HID usb_hid;
};

TUGamepad::TUGamepad(void)
{
  this->usb_hid.setPollInterval(1);
  this->usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  // setStringDescriptor is undefined on nRF52840
  this->usb_hid.setStringDescriptor("Winterjasper");
}

void TUGamepad::begin(void)
{
  this->usb_hid.begin();

  // release all buttons, center all sticks, etc.
  end();
  startMillis = millis();
}

void TUGamepad::loop(void)
{
  if (startMillis != millis()) {
    write();
    startMillis = millis();
  }
}

void TUGamepad::end(void)
{
  memset(&_report, 0x00, sizeof(_report));
  SendReport(&_report, sizeof(_report));
}

void TUGamepad::write(void)
{
  SendReport(&_report, sizeof(_report));
}

void TUGamepad::write(void *report)
{
  memcpy(&_report, report, sizeof(_report));
  SendReport(&_report, sizeof(_report));
}

void TUGamepad::press(uint8_t b)
{
  b &= 32;
  _report.buttons |= (uint32_t)1 << b;
}


void TUGamepad::release(uint8_t b)
{
  b &= 32;
  _report.buttons &= ~((uint32_t)1 << b);
}


void TUGamepad::releaseAll(void)
{
  _report.buttons = 0;
}

void TUGamepad::buttons(uint16_t b)
{
  _report.buttons = b;
}


void TUGamepad::leftXAxis(int8_t a)
{
  _report.axes[0] = a;
}


void TUGamepad::leftYAxis(int8_t a)
{
  _report.axes[1] = a;
}


void TUGamepad::rightXAxis(int8_t a)
{
  _report.axes[2] = a;
}


void TUGamepad::rightYAxis(int8_t a)
{
  //_report.axes[] = a;
}


TUGamepad Gamepad;

void setup()
{
  Gamepad.begin();
  Gamepad._report.buttons = 1;
  Gamepad._report.extra_buttons = 0;

  //USBDevice.setID(0x0f0d, 0x0092);

  // wait until device mounted
  while( !USBDevice.mounted() ) delay(1);
}


void loop() {
    if ( !Gamepad.ready() ) return;


    Gamepad._report.buttons <<= 1;

    if (Gamepad._report.buttons == 0) {
        Gamepad._report.buttons = 1;
    }

    Gamepad._report.extra_buttons += 1;

    // Move x/y Axis to a random position
    Gamepad.leftXAxis(random(256));
    Gamepad.rightXAxis(random(256));

    // Functions above only set the values.
    // This writes the report to the host.
    Gamepad.loop();
    delay(200);
}
