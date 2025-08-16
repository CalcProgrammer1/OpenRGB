/*---------------------------------------------------------*\
| SteelSeriesApexTZoneController.h                          |
|                                                           |
|   Driver for SteelSeries Apex T Zone                      |
|                                                           |
|   Edbgon                                      06 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesApex3Controller.h"

#define STEELSERIES_TZ_LED_COUNT            10
#define STEELSERIES_TZ_WRITE_PACKET_SIZE    33
#define STEELSERIES_TZ_BRIGHTNESS_MAX       0x64

class SteelSeriesApexTZoneController : public SteelSeriesApex3Controller
{
public:
    SteelSeriesApexTZoneController(hid_device *dev_handle, const char *path, std::string dev_name);
    ~SteelSeriesApexTZoneController();

    void        SetColor(std::vector<RGBColor> colors, uint8_t mode, uint8_t brightness);
    void        Save();
    uint8_t     GetLedCount();
    uint8_t     GetMaxBrightness();
    bool        SupportsRainbowWave();
    bool        SupportsSave();

private:

};
