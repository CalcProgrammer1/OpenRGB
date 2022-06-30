/*---------------------------------------------------------------------*\
|  BloodyMouseController.h                                              |
|                                                                       |
|  Driver for BloodyMouse USB Controller                                |
|                                                                       |
|  Chris M (Dr_No)          30 Jun 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>
#include "LogManager.h"
#include "RGBController.h"

#pragma once

#define HID_MAX_STR 255
#define BLOODYMOUSE_WRITE_PACKET_SIZE   64

#define BLOODYMOUSE_BRIGHTNESS_MIN      0
#define BLOODYMOUSE_BRIGHTNESS_MAX      255

enum
{
    BLOODYMOUSE_MODE_DIRECT             = 0x01, //Direct Led Control - Independently set LEDs in zone
};

enum
{
    BLOODYMOUSE_REPORT_BYTE             = 1,
    BLOODYMOUSE_COMMAND_BYTE            = 2,
    BLOODYMOUSE_MODE_BYTE               = 3,
    BLOODYMOUSE_DATA_BYTE               = 8,
};

class BloodyMouseController
{
public:
    BloodyMouseController(hid_device* dev_handle, const char* path);
    ~BloodyMouseController();

    std::string     GetSerial();
    std::string     GetLocation();

    void            SetLedsDirect(std::vector<RGBColor> colors);
private:
    std::string     location;
    hid_device*     dev;

    void            InitDevice();
};
