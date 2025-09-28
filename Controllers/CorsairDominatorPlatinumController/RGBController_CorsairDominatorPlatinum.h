/*---------------------------------------------------------*\
| RGBController_CorsairDominatorPlatinum.h                  |
|                                                           |
|   RGBController for Corsair Dominator Platinum RAM        |
|                                                           |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairDominatorPlatinumController.h"

class RGBController_CorsairDominatorPlatinum : public RGBController
{
public:
    RGBController_CorsairDominatorPlatinum(CorsairDominatorPlatinumController* controller_ptr);
    ~RGBController_CorsairDominatorPlatinum();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairDominatorPlatinumController* controller;
};
