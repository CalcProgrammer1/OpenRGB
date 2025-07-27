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
    HyperXAlloyOriginsController(hid_device* dev_handle, const char* path);
    ~HyperXAlloyOriginsController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    void            SaveSettings(std::vector<RGBColor> colors);
    void            SetLEDsDirect(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    std::string             location;

    void    write_led_data(std::vector<RGBColor> colors, int memory_loc, int modifier);

    void    SendCommandRaw(unsigned int val1, unsigned int val2, unsigned int val3);

    void    SendDirectInitialization(int memory_loc);

    void    SendDirectFinalization();

    void    SendDirectColorPacket
                (RGBColor*       color_data,
                unsigned int    color_count
                , int modifier);
};
