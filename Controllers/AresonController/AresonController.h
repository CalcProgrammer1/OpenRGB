/*---------------------------------------------------------*\
| AresonController.h                                        |
|                                                           |
|   Driver for Areson mice                                  |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define ARESON_PACKET_SIZE            17
#define ARESON_REPORT_ID              0x08
#define ARESON_PACKET_END             0x4A

enum
{
    RAINBOW_WAVE_MODE_VALUE         = 0x00,
    BREATHING_MODE_VALUE            = 0x01,
    STATIC_MODE_VALUE               = 0x02,
    SPECRTUM_CYCLE_MODE_VALUE       = 0x03,
    OFF_MODE_VALUE                  = 0x04,
    SINGLE_COLOR_WAVE_MODE_VALUE    = 0x05,
    BREATHING_COLORFUL_MODE_VALUE   = 0x07,
};


enum
{
    ARESON_BRIGHTNESS_MIN     = 1,
    ARESON_BRIGHTNESS_MAX     = 10,
    ARESON_SPEED_MIN          = 1,
    ARESON_SPEED_MAX          = 10
};

class AresonController
{
public:
    AresonController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~AresonController();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SetMode(RGBColor color, unsigned char brightness, unsigned char speed, unsigned char mode_value);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;
    unsigned char               GetSpeedValue(unsigned char speed, unsigned char mode_value);
};
