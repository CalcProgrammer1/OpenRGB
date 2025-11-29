/*---------------------------------------------------------*\
| SteelSeriesApex9Controller.h                              |
|                                                           |
|   Driver for SteelSeries Apex 9                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesApexBaseController.h"

enum
{
    APEX_9_PACKET_ID_DIRECT           = 0x40,     /* Direct mode                */
    APEX_9_PACKET_LENGTH              = 513,
};

class SteelSeriesApex9Controller : public SteelSeriesApexBaseController
{
public:
    SteelSeriesApex9Controller(hid_device* dev_handle, steelseries_type type, const char* path, std::string dev_name);
    ~SteelSeriesApex9Controller();

    void SetMode(unsigned char mode, std::vector<RGBColor> colors);
    void SetLEDsDirect(std::vector<RGBColor> colors);

    std::string GetSerial() override;
    std::string GetVersion() override;

private:
    void SelectProfile(unsigned char profile);
};
