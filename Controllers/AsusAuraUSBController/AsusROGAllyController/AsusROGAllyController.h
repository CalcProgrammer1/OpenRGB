/*---------------------------------------------------------*\
| AsusROGAllyController.h                                   |
|                                                           |
|   Driver for ASUS ROG Ally                                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

enum
{
    ROG_ALLY_MODE_STATIC           = 0,
    ROG_ALLY_MODE_BREATHING        = 1,
    ROG_ALLY_MODE_COLOR_CYCLE      = 2,
    ROG_ALLY_MODE_RAINBOW          = 3,
    ROG_ALLY_MODE_STROBING         = 10,
    ROG_ALLY_MODE_DIRECT           = 0xFF,
};

enum
{
    ROG_ALLY_SPEED_MIN              = 0xE1,
    ROG_ALLY_SPEED_MED              = 0xEB,
    ROG_ALLY_SPEED_MAX              = 0xF5
};

enum
{
    ROG_ALLY_DIRECTION_RIGHT        = 0x00,
    ROG_ALLY_DIRECTION_LEFT         = 0x01
};

class ROGAllyController
{
public:
    ROGAllyController(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~ROGAllyController();

    std::string GetDeviceLocation();
    std::string GetName();
    std::string GetSerialString();
    std::string GetVersion();

    void        SendInitialization();

    void        UpdateBrightness
                    (
                        unsigned char           brightness
                    );

    void        UpdateLeds
                    (
                    std::vector<RGBColor>    colors
                    );

    void        UpdateDevice
                    (
                    unsigned char           mode,
                    std::vector<RGBColor>   colors,
                    unsigned char           speed,
                    unsigned char           direction
                    );

    void        SaveMode();

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
