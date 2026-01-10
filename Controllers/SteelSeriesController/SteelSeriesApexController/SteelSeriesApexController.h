/*---------------------------------------------------------*\
| SteelSeriesApexController.cpp                             |
|                                                           |
|   Driver for SteelSeries Apex Keyboards                   |
|                                                           |
|   New driver based on SignalRGB Plugins                   |
|   https://gitlab.com/signalrgb/signal-plugins/            |
|                                                           |
|   Eric Samuelson (edbgon)                     05 Jul 2020 |
|   Filipe S. (filipesn)                         5 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesApexBaseController.h"

enum
{
    APEX_PACKET_ID_DIRECT               = 0x3a,     /* Direct mode                */
    APEX_2023_PACKET_ID_DIRECT          = 0x40,     /* New Wired Direct mode      */
    APEX_2023_PACKET_ID_DIRECT_WIRELESS = 0x61,     /* New Wireless Direct mode   */
    APEX_2023_PACKET_ID_INIT            = 0x4B,     /* New Initialization         */
    APEX_2023_PACKET_LENGTH             = 643,
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

    std::string GetSerial() override;

private:

    void    SelectProfile
                (
                unsigned char   profile
                );

    void    SendInitialization();

    bool    use_new_protocol;
};
