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

class LuxaforController
{
public:
    LuxaforController(hid_device* dev_handle, const char* path);
    ~LuxaforController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendPacket(unsigned char mode, unsigned char led, unsigned char red, unsigned char grn, unsigned char blu);

private:
    hid_device* dev;
    std::string location;
};
