/*---------------------------------------------------------*\
| LianLiUniversalScreenController.h                         |
|                                                           |
|   Driver for Lian Li 8.8" Universal Screen LEDs           |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      17 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <libusb.h>
#include "RGBController.h"

class LianLiUniversalScreenController
{
public:
    LianLiUniversalScreenController(libusb_device_handle* device);
    ~LianLiUniversalScreenController();

    std::string GetVersion();
    std::string GetLocation();
    std::string GetSerial();

    void        SetLedColors(RGBColor* colors, size_t count);

private:
    libusb_device_handle* dev;

    std::string version;
    std::string location;
    std::string serial;
};
