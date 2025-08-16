/*---------------------------------------------------------*\
| SteelSeriesApexController.h                               |
|                                                           |
|   Driver for SteelSeries Apex 7                           |
|                                                           |
|   Eric Samuelson (edbgon)                     05 Jul 2020 |
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
    APEX_PACKET_ID_DIRECT             = 0x3a,     /* Direct mode                */
};

class SteelSeriesApexController : public SteelSeriesApexBaseController
{
public:
    SteelSeriesApexController(hid_device* dev_handle, steelseries_type type, const char* path, std::string dev_name);
    ~SteelSeriesApexController();

    void SetMode
        (
        unsigned char mode,
        std::vector<RGBColor> colors
        );

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:

    void    SelectProfile
                (
                unsigned char   profile
                );
};
