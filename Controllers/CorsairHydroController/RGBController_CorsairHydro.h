/*---------------------------------------------------------*\
| RGBController_CorsairHydro.h                              |
|                                                           |
|   RGBController for Corsair Hydro Series coolers          |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      17 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairHydroController.h"

class RGBController_CorsairHydro : public RGBController
{
public:
    RGBController_CorsairHydro(CorsairHydroController* controller_ptr);
    ~RGBController_CorsairHydro();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairHydroController* controller;
};
