/*---------------------------------------------------------*\
| HyperXPulsefireFPSProController.h                         |
|                                                           |
|   Driver for HyperX Pulsefire FPS Pro                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                26 Dec 2020 |
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
    HYPERX_PULSEFIRE_FPS_PRO_PACKET_ID_DIRECT           = 0x0A,     /* Direct control packet                */
};

class HyperXPulsefireFPSProController
{
public:
    HyperXPulsefireFPSProController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXPulsefireFPSProController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
};
