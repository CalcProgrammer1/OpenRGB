/*---------------------------------------------------------*\
| RGBController_CorsairCCommanderCore.h                     |
|                                                           |
|   RGBController for Corsair Commander Core                |
|                                                           |
|   Jeff P.                                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairCommanderCoreController.h"

class RGBController_CorsairCommanderCore : public RGBController
{
public:
    RGBController_CorsairCommanderCore(CorsairCommanderCoreController* controller_ptr);
    ~RGBController_CorsairCommanderCore();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairCommanderCoreController*     controller;
    std::vector<int>                    fanleds{0};
};
