/*---------------------------------------------------------*\
| MSI3ZoneController.h                                      |
|                                                           |
|   Driver for MSI/SteelSeries 3-Zone keyboard              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class MSI3ZoneController
{
public:
    MSI3ZoneController(hid_device* dev_handle, const char* path);
    ~MSI3ZoneController();

    char*       GetDeviceName();
    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SetLEDs(std::vector<RGBColor> colors);

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
};
