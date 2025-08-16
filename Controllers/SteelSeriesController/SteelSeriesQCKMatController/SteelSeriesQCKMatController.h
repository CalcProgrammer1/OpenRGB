/*---------------------------------------------------------*\
| SteelSeriesQCKControllerMat.h                             |
|                                                           |
|   Driver for SteelSeries Mouse                            |
|                                                           |
|   Edbgon                                      22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class SteelSeriesQCKMatController
{
public:
    SteelSeriesQCKMatController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~SteelSeriesQCKMatController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    void        SetColors(std::vector<RGBColor> colors);

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
