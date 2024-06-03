/*---------------------------------------------------------*\
| RGBController_PatriotViperSteel.h                         |
|                                                           |
|   RGBController for Patriot Viper Steel RAM               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PatriotViperSteelController.h"

class RGBController_PatriotViperSteel : public RGBController
{
public:
    RGBController_PatriotViperSteel(PatriotViperSteelController *controller_ptr);
    ~RGBController_PatriotViperSteel();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    PatriotViperSteelController *controller;
};
