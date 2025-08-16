/*---------------------------------------------------------*\
| SteelSeriesApex3Controller.h                              |
|                                                           |
|   Driver for SteelSeries Apex 3                           |
|                                                           |
|   Chris M (Dr_No)                             23 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"

#define STEELSERIES_APEX3_BRIGHTNESS_MIN    0x00
#define STEELSERIES_APEX3_HID_TIMEOUT       100

class SteelSeriesApex3Controller
{
public:
    SteelSeriesApex3Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~SteelSeriesApex3Controller();

    std::string         GetDeviceLocation();
    std::string         GetNameString();
    std::string         GetSerialString();
    steelseries_type    GetKeyboardType();

    virtual void        SetColor(std::vector<RGBColor> colors, uint8_t mode, uint8_t brightness)    = 0;
    virtual void        Save()                                                                      = 0;
    virtual uint8_t     GetLedCount()                                                               = 0;
    virtual uint8_t     GetMaxBrightness()                                                          = 0;
    virtual bool        SupportsRainbowWave()                                                       = 0;
    virtual bool        SupportsSave()                                                              = 0;

    hid_device*         dev;

private:
    std::string         location;
    std::string         name;
};
