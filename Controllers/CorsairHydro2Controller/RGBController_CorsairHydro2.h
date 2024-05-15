/*---------------------------------------------------------*\
| RGBController_CorsairHydro2.h                             |
|                                                           |
|   RGBController for Corsair H100i v2                      |
|                                                           |
|   Tim Demand (tim.dmd)                        10 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairHydro2Controller* controller;
};
