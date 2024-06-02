/*---------------------------------------------------------*\
| RGBController_NZXTKraken.h                                |
|                                                           |
|   RGBController for NZXT Kraken                           |
|                                                           |
|   Martin Hartl (inlart)                       04 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <set>
#include "RGBController.h"
#include "NZXTKrakenController.h"

class RGBController_NZXTKraken : public RGBController
{
public:
    RGBController_NZXTKraken(NZXTKrakenController* controller_ptr);
    ~RGBController_NZXTKraken();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    std::vector<std::vector<RGBColor>> GetColors
        (
        int zone,
        const mode& channel_mode
        );

    void UpdateChannel
        (
        NZXTKrakenChannel_t channel,
        int zone,
        const mode& channel_mode
        );

    NZXTKrakenController*       controller;
    std::set<unsigned int>      logo_modes;
    int                         default_mode;
};
