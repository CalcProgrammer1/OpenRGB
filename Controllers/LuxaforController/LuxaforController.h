/*---------------------------------------------------------*\
| LuxaforController.h                                       |
|                                                           |
|   Driver for Luxafor devices                              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

enum
{
    LUXAFOR_LED_FIRST       = 1,
    LUXAFOR_LED_ALL         = 255,
};

enum
{
    LUXAFOR_MODE_DIRECT     = 1,
    LUXAFOR_MODE_FADE       = 2,
    LUXAFOR_MODE_STROBE     = 3,
    LUXAFOR_MODE_WAVE       = 4,
    LUXAFOR_MODE_PATTERN    = 6,
};

enum
{
    LUXAFOR_PATTERN_TRAFFIC_LIGHTS  = 1,
    LUXAFOR_PATTERN_2               = 2,
    LUXAFOR_PATTERN_3               = 3,
    LUXAFOR_PATTERN_4               = 4,
    LUXAFOR_PATTERN_POLICE          = 5,
    LUXAFOR_PATTERN_6               = 6,
    LUXAFOR_PATTERN_7               = 7,
    LUXAFOR_PATTERN_8               = 8,
};

class LuxaforController
{
public:
    LuxaforController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~LuxaforController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SendPacket(unsigned char mode, unsigned char led, unsigned char red, unsigned char grn, unsigned char blu, unsigned char type);

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
