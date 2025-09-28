/*---------------------------------------------------------*\
| SteelSeriesArctis5Controller.h                            |
|                                                           |
|   Driver for SteelSeries Arctis 5                         |
|                                                           |
|   Morgan Guimard                              04 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define ARCTIS_5_REPORT_SIZE 37
#define ARCTIS_5_REPORT_ID 0x06

class SteelSeriesArctis5Controller
{
public:
    SteelSeriesArctis5Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~SteelSeriesArctis5Controller();

    std::string         GetDeviceLocation();
    std::string         GetNameString();
    std::string         GetSerialString();

    void                SetColor(unsigned char zone_id, RGBColor color);

private:
    hid_device*         dev;
    std::string         location;
    std::string         name;
};
