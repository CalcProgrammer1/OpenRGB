/*---------------------------------------------------------*\
| RGBController_Espurna.h                                   |
|                                                           |
|   RGBController for Espurna                               |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EspurnaController.h"

class RGBController_Espurna : public RGBController
{
public:
    RGBController_Espurna(EspurnaController* controller_ptr);
    ~RGBController_Espurna();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EspurnaController* controller;
};
