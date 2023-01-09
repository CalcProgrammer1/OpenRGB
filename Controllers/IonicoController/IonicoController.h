#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#define IONICO_REPORT_SIZE                9
#define IONICO_DIRECT_REPORT_SIZE         65
#define IONICO_DIRECT_CMD                 0x14
#define IONICO_KEYBOARD_LED_COUNT         4
#define IONICO_BAR_LED_COUNT              22
#define IONICO_DIRECT_BRIGHTNESS_MIN      0
#define IONICO_DIRECT_BRIGHTNESS_MAX      50
#define IONICO_DIRECT_SPEED_MIN           0
#define IONICO_DIRECT_SPEED_MAX           10
#define IONICO_DIRECT_SPEED_DEFAULT       5

enum
{
    IONICO_MODE_OFF             = 0,
    IONICO_MODE_DIRECT          = 1,
    IONICO_MODE_BREATHING       = 2,
    IONICO_MODE_WAVE            = 3,
    IONICO_MODE_RAIN            = 10,
    IONICO_MODE_FLASH           = 18,
    IONICO_FB_MODE_WAVE         = 32
};


class IonicoController
{
    public:
        IonicoController(hid_device* dev_handle, const hid_device_info& info, const unsigned short pid);
        ~IonicoController();

        std::string GetDeviceLocation();

        void        SetMode(uint8_t mode_value, uint8_t brightness, uint8_t speed);
        void        SetColors(int device, std::vector<RGBColor> array_colors, bool is_mode);
        void        SaveBios();
        void        TurnOff();
        uint16_t    GetUSBPID();

    private:
        hid_device* dev;
        std::string location;
        std::string serial_number;
        std::string version;
        uint16_t    usb_pid;
};
