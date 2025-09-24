/*---------------------------------------------------------*\
| RGBController_PatriotViper.h                              |
|                                                           |
|   RGBController for Patriot Viper RAM                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PatriotViperController* viper;
};
