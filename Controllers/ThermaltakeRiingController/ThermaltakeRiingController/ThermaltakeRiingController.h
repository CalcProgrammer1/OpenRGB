/*---------------------------------------------------------*\
| ThermaltakeRiingController.h                              |
|                                                           |
|   Driver for Thermaltake Riing                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    THERMALTAKE_PORT_1              = 0x01,
    THERMALTAKE_PORT_2              = 0x02,
    THERMALTAKE_PORT_3              = 0x03,
    THERMALTAKE_PORT_4              = 0x04,
    THERMALTAKE_PORT_5              = 0x05
};

enum
{
    THERMALTAKE_MODE_FLOW           = 0x00,
    THERMALTAKE_MODE_SPECTRUM       = 0x04,
    THERMALTAKE_MODE_RIPPLE         = 0x08,
    THERMALTAKE_MODE_BLINK          = 0x0C,
    THERMALTAKE_MODE_PULSE          = 0x10,
    THERMALTAKE_MODE_WAVE           = 0x14,
    THERMALTAKE_MODE_PER_LED        = 0x18,
    THERMALTAKE_MODE_FULL           = 0x19
};

enum
{
    THERMALTAKE_SPEED_SLOW          = 0x03,
    THERMALTAKE_SPEED_NORMAL        = 0x02,
    THERMALTAKE_SPEED_FAST          = 0x01,
    THERMALTAKE_SPEED_EXTREME       = 0x00
};

#define THERMALTAKE_NUM_CHANNELS    5

class ThermaltakeRiingController
{
public:
    ThermaltakeRiingController(hid_device* dev_handle, const char* path);
    ~ThermaltakeRiingController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    std::string     GetFirmwareVersion();

    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);
    void            SetMode(unsigned char mode, unsigned char speed);

private:
    hid_device*     dev;

    unsigned char   current_mode;
    unsigned char   current_speed;
    std::string     location;

    void    SendInit();

    void SendRGB
        (
            unsigned char       port,
            unsigned char       mode,
            unsigned char       speed,
            unsigned char       num_colors,
            unsigned char*      color_data
        );

    void    SendFan();
    void    SendSave();
};
