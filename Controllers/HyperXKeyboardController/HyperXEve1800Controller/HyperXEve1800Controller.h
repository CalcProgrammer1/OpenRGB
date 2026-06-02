/*---------------------------------------------------------*\
| HyperXEve1800Controller.h                                |
|                                                           |
|   Driver for HyperX Eve 1800 keyboard                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class HyperXEve1800Controller
{
public:
    HyperXEve1800Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXEve1800Controller();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SetBrightness(unsigned int brightness);
    void            SetLEDsDirect(std::vector<RGBColor> colors);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;

    void            SendDirectInitialization();
    void            SendDirectColorPacket(RGBColor* color_data, unsigned int color_count);
};
