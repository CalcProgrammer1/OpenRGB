/*---------------------------------------------------------*\
| RGBController_RazerAddressable.h                          |
|                                                           |
|   RGBController for Razer ARGB Controller                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RazerController.h"

enum
{
    RAZER_ADDRESSABLE_MODE_DIRECT,
    RAZER_ADDRESSABLE_MODE_OFF,
    RAZER_ADDRESSABLE_MODE_STATIC,
    RAZER_ADDRESSABLE_MODE_BREATHING,
    RAZER_ADDRESSABLE_MODE_SPECTRUM_CYCLE,
    RAZER_ADDRESSABLE_MODE_WAVE,
    RAZER_ADDRESSABLE_MODE_REACTIVE,
};

class RGBController_RazerAddressable : public RGBController
{
public:
    RGBController_RazerAddressable(RazerController* controller_ptr);
    ~RGBController_RazerAddressable();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RazerController*    controller;
};
