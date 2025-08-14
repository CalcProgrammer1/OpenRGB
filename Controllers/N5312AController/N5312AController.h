/*---------------------------------------------------------*\
| N5312AController.h                                        |
|                                                           |
|   Driver for N5312A                                       |
|                                                           |
|   Morgan Guimard (morg)                       02 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define N5312A_REPORT_ID           0x07
#define N5312A_PACKET_DATA_LENGTH  8
#define N5312A_NUMBER_OF_LEDS      1
#define N5312A_INIT_BYTE           0xA0
#define N5312A_SET_MODE_BYTE       0x0A
#define N5312A_SET_COLOR_BYTE      0x0B

enum
{
    N5312A_BREATHING_MODE_VALUE         = 0x00,
    N5312A_SINGLE_BREATH_MODE_VALUE     = 0x01,
    N5312A_DIRECT_MODE_VALUE            = 0x02,
    N5312A_OFF_MODE_VALUE               = 0x03
};

enum
{
    N5312A_BRIGHTNESS_MIN               = 0x0A,
    N5312A_BRIGHTNESS_MAX               = 0x64,
    N5312A_SPEED_MIN                    = 0x01,
    N5312A_SPEED_MAX                    = 0x0A
};

class N5312AController
{
public:
    N5312AController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~N5312AController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        SetColor(RGBColor color);
    void        SetMode(RGBColor color, unsigned char mode_value, unsigned char brightness, unsigned char speed);

private:
    hid_device* dev;

    std::string location;
    std::string name;
    std::string version;

    void        SendInit();
};
