/*---------------------------------------------------------*\
| HyperXPulsefireHasteController.h                          |
|                                                           |
|   Driver for HyperX Pulsefire Haste                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Aug 2020 |
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
    HYPERX_PULSEFIRE_HASTE_PACKET_ID_SETUP              = 0x04,     /* Direct setup packet                  */
    HYPERX_PULSEFIRE_HASTE_PACKET_ID_COLOR              = 0x81,     /* Direct color packet                  */
};

class HyperXPulsefireHasteController
{
public:
    HyperXPulsefireHasteController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXPulsefireHasteController();

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

    void SendDirectSetup();
    void SendDirectColor
            (
            RGBColor*       color_data
            );
};
