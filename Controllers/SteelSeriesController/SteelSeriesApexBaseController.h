/*---------------------------------------------------------*\
| SteelSeriesApexBaseController.h                           |
|                                                           |
|   Driver base for SteelSeries Apex                        |
|                                                           |
|   Florian Heilmann (FHeilmann)                19 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <string>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"

#define STEELSERIES_PACKET_IN_SIZE  64
#define STEELSERIES_PACKET_OUT_SIZE STEELSERIES_PACKET_IN_SIZE + 1

/*-------------------------------------------------*\
| Gen 1: 2019-22 models (all FW) & 2023 FW < 1.19.7 |
| Gen 2: 2023 models with FW >= 1.19.7              |
| Gen 3: 2025+ and may feature Gen 3 in the name    |
\*-------------------------------------------------*/

typedef enum
{
    APEX_GEN1                 = 0x00,
    APEX_GEN2                 = 0x01,
    APEX_GEN3                 = 0x02,

} protocol_quirk;

class SteelSeriesApexBaseController
{
public:
    SteelSeriesApexBaseController(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~SteelSeriesApexBaseController();

    std::string         GetLocation();
    std::string         GetName();
    virtual std::string GetSerial();
    virtual std::string GetVersion();

    virtual void        SetMode(unsigned char mode, std::vector<RGBColor> colors)  = 0;

    virtual void        SetLEDsDirect(std::vector<RGBColor> colors)                = 0;

    steelseries_type    proto_type;

protected:
    hid_device*         dev;
    unsigned char       active_mode;
    std::string         location;
    std::string         name;
    protocol_quirk      kbd_quirk;
};
