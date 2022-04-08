/*-------------------------------------------------------------------*\
|  DarkProjectKeyboardController.h                                    |
|                                                                     |
|  Driver for DarkProjectKeyboard USB Controller                      |
|                                                                     |
|  Chris M (DrNo)          8 Apr 2022                                 |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>
#include "RGBController.h"

#pragma once

#define NA                                      0xFFFFFFFF
#define HID_MAX_STR                             255

#define DARKPROJECTKEYBOARD_PACKET_SIZE         256
#define DARKPROKECTKEYBOARD_TKL_KEYCOUNT        87

enum
{
    DARKPROJECTKEYBOARD_MODE_DIRECT     = 0x01,   //Direct Led Control - Independently set LEDs in zone
};

enum
{
    DARKPROJECTKEYBOARD_REPORT_BYTE     = 1,
    DARKPROJECTKEYBOARD_COMMAND_BYTE    = 2,
    DARKPROJECTKEYBOARD_RED_BLUE_BYTE   = 5,
    DARKPROJECTKEYBOARD_GREEN_BYTE      = 107
};

class DarkProjectKeyboardController
{
public:
    DarkProjectKeyboardController(hid_device* dev_handle, const char* path);
    ~DarkProjectKeyboardController();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SetLedsDirect(std::vector<RGBColor> colors);
private:
    std::string     location;
    hid_device*     dev;
};
