/*---------------------------------------------------------*\
| RGBController_CorsairVoid.h                               |
|                                                           |
|   RGBController for Corsair Void Pro headsets             |
|                                                           |
|   Derek Ross                                  15 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairVoidController.h"

class RGBController_CorsairVoid : public RGBController
{
public:
    RGBController_CorsairVoid(CorsairVoidController* controller_ptr);
    ~RGBController_CorsairVoid();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairVoidController* controller;
};
