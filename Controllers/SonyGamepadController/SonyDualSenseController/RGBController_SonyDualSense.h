/*---------------------------------------------------------*\
| RGBController_SonyDualSense.h                             |
|                                                           |
|   RGBController for Sony DualSense                        |
|                                                           |
|   Flora Aubry                                 01 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SonyDualSenseController.h"

class RGBController_SonyDualSense : public RGBController
{
public:
    RGBController_SonyDualSense(SonyDualSenseController* controller_ptr);
    ~RGBController_SonyDualSense();

    void        SetupZones();
    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SonyDualSenseController* controller;
};
