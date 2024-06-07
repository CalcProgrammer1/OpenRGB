/*---------------------------------------------------------*\
| RGBController_RazerKraken.h                               |
|                                                           |
|   RGBController for Razer Kraken                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                28 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RazerKrakenController.h"

enum
{
    RAZER_KRAKEN_MODE_DIRECT,
    RAZER_KRAKEN_MODE_OFF,
    RAZER_KRAKEN_MODE_STATIC,
    RAZER_KRAKEN_MODE_BREATHING,
    RAZER_KRAKEN_MODE_SPECTRUM_CYCLE,
};

class RGBController_RazerKraken : public RGBController
{
public:
    RGBController_RazerKraken(RazerKrakenController* controller_ptr);
    ~RGBController_RazerKraken();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RazerKrakenController*  controller;
};
