/*---------------------------------------------------------*\
| RGBController_LegoDimensionsToypadBase.h                  |
|                                                           |
|   RGBController for Lego Dimensions Toypad Base           |
|                                                           |
|   Morgan Guimard (morg)                       02 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LegoDimensionsToypadBaseController.h"

class RGBController_LegoDimensionsToypadBase : public RGBController
{
public:
    RGBController_LegoDimensionsToypadBase(LegoDimensionsToypadBaseController* controller_ptr);
    ~RGBController_LegoDimensionsToypadBase();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LegoDimensionsToypadBaseController* controller;
};
