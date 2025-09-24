/*---------------------------------------------------------*\
| RGBController_CorsairHydro2.h                             |
|                                                           |
|   RGBController for Corsair H100i v2                      |
|                                                           |
|   Tim Demand (tim.dmd)                        10 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairHydro2Controller.h"

class RGBController_CorsairHydro2 : public RGBController
{
public:
    RGBController_CorsairHydro2(CorsairHydro2Controller* controller_ptr);
    ~RGBController_CorsairHydro2();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairHydro2Controller* controller;
};
