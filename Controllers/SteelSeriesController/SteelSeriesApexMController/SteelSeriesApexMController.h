/*---------------------------------------------------------*\
| SteelSeriesApexMController.cpp                            |
|                                                           |
|   Driver for SteelSeries Apex M750                        |
|                                                           |
|   Florian Heilmann (FHeilmann)                12 Oct 2020 |
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

class SteelSeriesApexMController : public SteelSeriesApexBaseController
{
public:
    SteelSeriesApexMController(hid_device* dev_handle, steelseries_type type, const char* path, std::string dev_name);
    ~SteelSeriesApexMController();

    void SetMode
        (
            unsigned char mode,
            std::vector<RGBColor> colors
        );

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:
    void EnableLEDControl();
    void SelectProfile
        (
            unsigned char   profile
        );
};
