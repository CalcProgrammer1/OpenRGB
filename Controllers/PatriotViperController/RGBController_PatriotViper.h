/*---------------------------------------------------------*\
| RGBController_PatriotViper.h                              |
|                                                           |
|   RGBController for Patriot Viper RAM                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PatriotViperController.h"

class RGBController_PatriotViper : public RGBController
{
public:
    RGBController_PatriotViper(PatriotViperController* viper_ptr);
    ~RGBController_PatriotViper();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PatriotViperController* viper;
};
