/*---------------------------------------------------------*\
| HyperXAlloyOriginsController.h                            |
|                                                           |
|   Driver for HyperX Alloy Origins keyboard                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class HyperXAlloyOriginsController
{
public:
    HyperXAlloyOriginsController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXAlloyOriginsController();

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
