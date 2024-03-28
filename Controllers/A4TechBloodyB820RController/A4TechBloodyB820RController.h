/*-------------------------------------------------------------------*\
|  A4TechBloodyB820RController.h                                    |
|                                                                     |
|  Driver for DarkProjectKeyboard USB Controller                      |
|                                                                     |
|  Zulfikar (o-julfikar)          8 Apr 2022                                 |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>
#include "RGBController.h"

#pragma once

#define NA                                      0xFFFFFFFF
#define HID_MAX_STR                             255

#define BLOODY_B820R_PACKET_SIZE         64
#define BLOODY_B820R_TKL_KEYCOUNT        87

enum
{
    BLOODY_B820R_MODE_DIRECT     = 0x01,   //Direct Led Control - Independently set LEDs in zone
};

enum
{
    BLOODY_B820R_REPORT_BYTE     = 1,
    BLOODY_B820R_COMMAND_BYTE    = 2,
    BLOODY_B820R_RED_BLUE_BYTE   = 5,
    BLOODY_B820R_GREEN_BYTE      = 107
};

class A4TechBloodyB820RController
{
public:
    A4TechBloodyB820RController(hid_device* dev_handle, const char* path);
    ~A4TechBloodyB820RController();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SetLedsDirect(std::vector<RGBColor> colors);
private:
    std::string     location;
    hid_device*     dev;
};
