/*---------------------------------------------------------*\
| HyperXAlloyOrigins60and65Controller.h                     |
|                                                           |
|   Driver for HyperX Alloy Origins 60 and 65 keyboard      |
|                                                           |
|   Derek Huber                                 18 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class HyperXAlloyOrigins60and65Controller
{
public:
    HyperXAlloyOrigins60and65Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXAlloyOrigins60and65Controller();

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
                unsigned int    color_count
                );
};
