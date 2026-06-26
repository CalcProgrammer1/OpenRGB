/*---------------------------------------------------------*\
| SayoDeviceController.h                                    |
|                                                           |
|   Controller for Sayo Devices                             |
|                                                           |
|   Richard Harris                              24 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------*\
| Lighting modes                            |
\*-----------------------------------------*/
enum
{
    SAYO_MODE_STATIC                = 0x00,
    SAYO_MODE_INDICATOR             = 0x01,
    SAYO_MODE_BREATHING             = 0x02,
    SAYO_MODE_BREATHING_ONCE        = 0x03,
    SAYO_MODE_WAVE                  = 0x04,
    SAYO_MODE_SWITCH                = 0x06,
    SAYO_MODE_SWITCH_ONCE           = 0x07,
    SAYO_MODE_BLINK                 = 0x08,
    SAYO_MODE_BLINK_ONCE            = 0x09,
    SAYO_MODE_FADE_OUT              = 0x0E,
    SAYO_MODE_FADE_IN               = 0x0F,
    /*-------------------------------------*\
    | Virtual modes                         |
    \*-------------------------------------*/
    SAYO_MODE_DIRECT                = 0xFF,
};

/*-----------------------------------------*\
| Animation speeds                          |
\*-----------------------------------------*/
enum
{
    SAYO_SPEED_1X                   = 3,
    SAYO_SPEED_2X                   = 2,
    SAYO_SPEED_4X                   = 1,
    SAYO_SPEED_8X                   = 0,
};

/*-----------------------------------------*\
| Animation color mode                      |
| TABLE modes either loop through or pick   |
| randomly from a palette of colors.        |
| RANDOM is truly random.                   |
\*-----------------------------------------*/
enum
{
    SAYO_COLOR_STATIC               = 0,
    SAYO_COLOR_LOOP_TABLE           = 1,
    SAYO_COLOR_RANDOM_TABLE         = 2,
    SAYO_COLOR_RANDOM               = 3,
};

#define SAYO_MODE_PACK(speed, color_mode, mode) \
    ((unsigned char)((((speed) & 0x3) << 6) | \
                    (((color_mode) & 0x3) << 4) | \
                    ((mode) & 0xF)))

/*-----------------------------------------*\
| Commands                                  |
\*-----------------------------------------*/
enum
{
    SAYO_CMD_API_LIST               = 0x00,
    SAYO_CMD_KEY_GET                = 0x02, // maybe?
    SAYO_CMD_SETTINGS               = 0x03,
    SAYO_CMD_KEY_SET                = 0x10,
    SAYO_CMD_LIGHTING_SET           = 0x11,
    SAYO_CMD_REBOOT                 = 0x0E,

};

class SayoDeviceController
{
public:
    SayoDeviceController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~SayoDeviceController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();

    void        SetMode(unsigned int mode, unsigned int speed, RGBColor color, bool random);
    void        Save();

private:
    hid_device*             dev;
    std::string             name;
    std::string             location;

    void        SendPacket(const std::vector<unsigned char>& command, bool flush = true);
};
