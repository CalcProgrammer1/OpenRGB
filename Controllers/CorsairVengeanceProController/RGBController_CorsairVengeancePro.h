/*---------------------------------------------------------*\
| RGBController_CorsairVengeancePro.h                       |
|                                                           |
|   RGBController for Corsair Vengeance Pro RGB RAM         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairVengeanceProController.h"

class RGBController_CorsairVengeancePro : public RGBController
{
public:
    RGBController_CorsairVengeancePro(CorsairVengeanceProController* controller_ptr);
    ~RGBController_CorsairVengeancePro();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairVengeanceProController* controller;
};
