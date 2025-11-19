/*---------------------------------------------------------*\
| RGBController_RazerKrakenV3.h                             |
|                                                           |
|   RGBController for Razer devices with 13-byte reports    |
|                                                           |
|   Greg Sandstrom (superstrom)                  1 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RazerKrakenV3Controller.h"

enum
{
    RAZER_KRAKEN_V3_MODE_DIRECT,
    RAZER_KRAKEN_V3_MODE_WAVE,
    RAZER_KRAKEN_V3_MODE_BREATHING,
};

class RGBController_RazerKrakenV3 : public RGBController
{
public:
    RGBController_RazerKrakenV3(RazerKrakenV3Controller* controller_ptr);
    ~RGBController_RazerKrakenV3();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RazerKrakenV3Controller*  controller;
};
