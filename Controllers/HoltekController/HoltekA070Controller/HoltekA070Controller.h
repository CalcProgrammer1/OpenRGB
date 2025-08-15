/*---------------------------------------------------------*\
| HoltekA070Controller.h                                    |
|                                                           |
|   Driver for Holtek mouse                                 |
|                                                           |
|   Santeri Pikarinen (santeri3700)             01 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

enum
{
    HOLTEK_A070_MODE_STATIC            = 0x01,
    HOLTEK_A070_MODE_BREATHING_SLOW    = 0x02,
    HOLTEK_A070_MODE_BREATHING_MEDIUM  = 0x03,
    HOLTEK_A070_MODE_BREATHING_FAST    = 0x04
};

class HoltekA070Controller
{
public:
    HoltekA070Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HoltekA070Controller();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        SendCustomColor
                   (
                   unsigned char       red,
                   unsigned char       green,
                   unsigned char       blue
                   );

    void        SendMode
                   (
                   unsigned char       mode
                   );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
};
