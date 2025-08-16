/*---------------------------------------------------------*\
| SteelSeriesSiberiaController.h                            |
|                                                           |
|   Driver for SteelSeries Siberia                          |
|                                                           |
|   E Karlsson (pilophae)                       18 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

class SteelSeriesSiberiaController
{
public:
    SteelSeriesSiberiaController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~SteelSeriesSiberiaController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    void        SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
