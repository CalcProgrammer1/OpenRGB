/*---------------------------------------------------------*\
| HyperXAlloyFPSController.h                                |
|                                                           |
|   Driver for HyperX Alloy FPS keyboard                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    HYPERX_ALLOY_FPS_PACKET_ID_DIRECT               = 0x16,     /* Direct control packet                */
};

enum
{
    HYPERX_ALLOY_FPS_COLOR_CHANNEL_RED              = 0x01,
    HYPERX_ALLOY_FPS_COLOR_CHANNEL_GREEN            = 0x02,
    HYPERX_ALLOY_FPS_COLOR_CHANNEL_BLUE             = 0x03
};

class HyperXAlloyFPSController
{
public:
    HyperXAlloyFPSController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXAlloyFPSController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;

    void    SendDirect
                (
                unsigned char   color_channel,
                unsigned char*  color_data
                );
};
