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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairHydroController* controller;
};
