/*---------------------------------------------------------*\
| RGBController_ElgatoKeyLight.h                            |
|                                                           |
|   RGBController for Elgato Key Light                      |
|                                                           |
|   Monks (@iamtherealestmonkey)                01 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ElgatoKeyLightController.h"

class RGBController_ElgatoKeyLight : public RGBController
{
public:
    RGBController_ElgatoKeyLight(ElgatoKeyLightController* controller_ptr);
    ~RGBController_ElgatoKeyLight();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ElgatoKeyLightController* controller;
};
