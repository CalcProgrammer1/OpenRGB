/*---------------------------------------------------------*\
| RGBController_CorsairHydroPlatinum.h                      |
|                                                           |
|   RGBController for Corsair Hydro Platinum coolers        |
|                                                           |
|   Kasper                                      28 Mar 2021 |
|   Nikola Jurkovic (jurkovic.nikola)           13 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairHydroPlatinumController.h"

class RGBController_CorsairHydroPlatinum : public RGBController
{
public:
    RGBController_CorsairHydroPlatinum(CorsairHydroPlatinumController* controller_ptr);
    ~RGBController_CorsairHydroPlatinum();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairHydroPlatinumController* controller;

    void        Init_Controller();
};
