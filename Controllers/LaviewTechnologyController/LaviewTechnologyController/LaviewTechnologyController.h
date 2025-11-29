/*---------------------------------------------------------*\
| LaviewTechnologyController.h                              |
|                                                           |
|   Driver for Laview Tech. mice, including Glorious        |
|                                                           |
|   Kosta A (kostaarvanitis)                    27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define LAVIEW_TECHNOLOGY_REPORT_SIZE   64

enum
{
    LAVIEW_TECHNOLOGY_MODE_OFF                  = 0x00, // Off
    LAVIEW_TECHNOLOGY_MODE_STATIC               = 0x01, // Normally on
    LAVIEW_TECHNOLOGY_MODE_FLASHING             = 0x02, // Wave
    LAVIEW_TECHNOLOGY_MODE_BREATHING            = 0x04, // Breathing single color
    LAVIEW_TECHNOLOGY_MODE_SPECTRUM_CYCLE       = 0x06, // Breating does not work with factory Core software
    LAVIEW_TECHNOLOGY_MODE_RAINBOW_WAVE         = 0x10, // Glorious Mode
    LAVIEW_TECHNOLOGY_MODE_CHASE                = 0x11, // Tail
    LAVIEW_TECHNOLOGY_MODE_WAVE                 = 0x14, // Rave (Same issue as breathing)
    LAVIEW_TECHNOLOGY_MODE_SPECTRUM_BREATHING   = 0x15, // Seamless Breathing
};

enum
{
    LAVIEW_TECHNOLOGY_SPEED_SLOW                = 1,
    LAVIEW_TECHNOLOGY_SPEED_NORMAL              = 50,
    LAVIEW_TECHNOLOGY_SPEED_FAST                = 100,
};

enum
{
    LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW           = 0,
    LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL         = 50,
    LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST           = 100,
};

class LaviewTechnologyController
{
public:
    LaviewTechnologyController(hid_device* dev_handle, hid_device_info* dev_info, std::string name);
    ~LaviewTechnologyController();

    std::string     GetLocation();
    std::string     GetName();
    std::string     GetVendor();
    std::string     GetSerialNumber();
    std::string     GetFirmwareVersion();

    void            SetMode(unsigned int mode, unsigned int brightness, unsigned int speed, RGBColor* color);

private:
    hid_device*     device;
    std::string     location;
    std::string     name;
    std::string     vendor;
    std::string     serial;
    unsigned int    version;
};
