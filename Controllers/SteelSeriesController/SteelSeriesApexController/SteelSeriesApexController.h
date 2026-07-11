/*---------------------------------------------------------*\
| SteelSeriesApexController.h                               |
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
#include "SteelSeriesDevices.h"
#include "SteelSeriesApexBaseController.h"

class SteelSeriesApexController : public SteelSeriesApexBaseController
{
public:
    SteelSeriesApexController(hid_device* dev_handle, steelseries_type type, const char* path, unsigned short pid, std::string dev_name);
    ~SteelSeriesApexController();
    void SetMode(unsigned char mode, std::vector<RGBColor> colors) override;
    void SetLEDsDirect(std::vector<RGBColor> colors) override;
    std::string GetSerial() override;

private:
    void SelectProfile(unsigned char profile);
    void SendInitialization();
    void SendDeinitialization();
};
