/*---------------------------------------------------------*\
| ClevoLightbarController.h                                 |
|                                                           |
|   Driver for Clevo laptop lightbar (ITE 8291 rev 0.03)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi.h>

#define CLEVO_LIGHTBAR_REPORT_SIZE         8
#define CLEVO_LIGHTBAR_BRIGHTNESS_MIN      0
#define CLEVO_LIGHTBAR_BRIGHTNESS_MAX      100
#define CLEVO_LIGHTBAR_SPEED_MIN           1
#define CLEVO_LIGHTBAR_SPEED_MAX           10
#define CLEVO_LIGHTBAR_SPEED_DEFAULT       5

enum
{
    CLEVO_LIGHTBAR_MODE_DIRECT     = 0x01,
    CLEVO_LIGHTBAR_MODE_BREATHING  = 0x02,
    CLEVO_LIGHTBAR_MODE_WAVE       = 0x03,
    CLEVO_LIGHTBAR_MODE_BOUNCE     = 0x04,
    CLEVO_LIGHTBAR_MODE_MARQUEE    = 0x05,
    CLEVO_LIGHTBAR_MODE_SCAN       = 0x06,
    CLEVO_LIGHTBAR_MODE_OFF        = 0x00
};

class ClevoLightbarController
{
public:
    ClevoLightbarController(hid_device* dev_handle, const hid_device_info& info);
    ~ClevoLightbarController();

    std::string GetDeviceLocation();
    std::string GetSerialString();
    std::string GetFirmwareVersion();

    void        SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void        SetBrightness(unsigned char brightness);
    void        SetMode(unsigned char mode, unsigned char brightness, unsigned char speed);
    void        TurnOn();
    void        TurnOff();

private:
    hid_device*     dev;
    std::string     location;
    unsigned short  version;

    void        WriteControl(unsigned char* data);
};
