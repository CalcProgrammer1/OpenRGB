/*---------------------------------------------------------*\
| HyperXOrigins2_65Controller.h                             |
|                                                           |
|   Driver for HyperX Origins 2 65 keyboard                 |
|                                                           |
|   Ricardo Amorim                              28 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class HyperXOrigins2_65Controller
{
public:
    HyperXOrigins2_65Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXOrigins2_65Controller();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;

    void    SendDirectInitialization();
    void    SendDirectColorPacket
                (
                RGBColor*       color_data,
                unsigned int    color_count,
                unsigned int    seq
                );
};

