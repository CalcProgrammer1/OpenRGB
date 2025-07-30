/*---------------------------------------------------------*\
| RGBController_PatriotViperMouse.h                         |
|                                                           |
|   RGBController for Patriot Viper Mouse                   |
|                                                           |
|   mi4code                                     07 Apr 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PatriotViperMouseController.h"

class RGBController_PatriotViperMouse : public RGBController
{
public:
    RGBController_PatriotViperMouse(PatriotViperMouseController* controller_ptr);
    ~RGBController_PatriotViperMouse();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PatriotViperMouseController* controller;
};
