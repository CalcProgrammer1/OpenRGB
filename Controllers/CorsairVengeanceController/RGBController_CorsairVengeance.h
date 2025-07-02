/*---------------------------------------------------------*\
| RGBController_CorsairVengeance.h                          |
|                                                           |
|   RGBController for original single-zone Corsair          |
|   Vengeance DDR4 RGB RAM                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                16 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairVengeanceController.h"

class RGBController_CorsairVengeance : public RGBController
{
public:
    RGBController_CorsairVengeance(CorsairVengeanceController* controller_ptr);
    ~RGBController_CorsairVengeance();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairVengeanceController* controller;
};
