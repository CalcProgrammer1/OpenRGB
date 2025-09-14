/*---------------------------------------------------------*\
| LogitechG600Controller.h                                  |
|                                                           |
|   Driver for Logitech G600 Gaming Mouse                   |
|                                                           |
|   Austin B     (austinleroy)                11 Sep 2025   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"


enum
{
    LOGITECH_G600_MODE_DIRECT            = 0x00,
    LOGITECH_G600_MODE_BREATHING         = 0x01,
    LOGITECH_G600_MODE_CYCLE             = 0x02
};

/*---------------------------------------------------------------------------------------------*\
| Speed is number of seconds for cycle to complete.                                             |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G600_SPEED_SLOWEST          = 0x0F,     /* Slowest speed                       */
    LOGITECH_G600_SPEED_NORMAL           = 0x03,     /* Normal speed                        */
    LOGITECH_G600_SPEED_FASTEST          = 0x01,     /* Fastest speed                       */
};

class LogitechG600Controller
{
public:
    LogitechG600Controller(hid_device* dev, const char* path, std::string dev_name);
    ~LogitechG600Controller();

    std::string GetNameString();
    std::string GetSerialString();

    void        SetMode
                    (
                    unsigned char       mode,
                    unsigned short      speed,
                    RGBColor            color
                    );

private:
    hid_device* dev;
    std::string location;
    std::string name;
};