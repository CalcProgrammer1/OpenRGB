/*---------------------------------------------------------*\
| RGBController_PatriotViperSteel.h                         |
|                                                           |
|   RGBController for Patriot Viper Steel RAM               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    PatriotViperSteelController *controller;
};
