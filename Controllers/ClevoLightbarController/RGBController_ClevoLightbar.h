/*---------------------------------------------------------*\
| RGBController_ClevoLightbar.h                             |
|                                                           |
|   Generic RGB Interface for Clevo laptop lightbar         |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ClevoLightbarController.h"

class RGBController_ClevoLightbar : public RGBController
{
public:
    RGBController_ClevoLightbar(ClevoLightbarController* controller_ptr);
    ~RGBController_ClevoLightbar();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ClevoLightbarController* controller;
};
