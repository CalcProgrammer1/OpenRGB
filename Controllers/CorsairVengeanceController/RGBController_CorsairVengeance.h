/*---------------------------------------------------------*\
| RGBController_CorsairVengeance.h                          |
|                                                           |
|   RGBController for original single-zone Corsair          |
|   Vengeance DDR4 RGB RAM                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                16 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairVengeanceController* controller;
};
