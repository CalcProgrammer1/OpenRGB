/*---------------------------------------------------------*\
| SteelSeriesApexBaseController.h                           |
|                                                           |
|   Driver base for SteelSeries Apex                        |
|                                                           |
|   Florian Heilmann (FHeilmann)                19 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <string>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"

#define STEELSERIES_PACKET_IN_SIZE  64
#define STEELSERIES_PACKET_OUT_SIZE STEELSERIES_PACKET_IN_SIZE + 1

class SteelSeriesApexBaseController
{
public:
    SteelSeriesApexBaseController(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~SteelSeriesApexBaseController();

    std::string         GetLocation();
    std::string         GetName();
    std::string         GetSerial();
    std::string         GetVersion();

    virtual void        SetMode(unsigned char mode, std::vector<RGBColor> colors)  = 0;

    virtual void        SetLEDsDirect(std::vector<RGBColor> colors)                = 0;

    steelseries_type    proto_type;

protected:
    hid_device*         dev;
    unsigned char       active_mode;
    std::string         location;
    std::string         name;
};
