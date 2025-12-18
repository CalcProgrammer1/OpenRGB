/*---------------------------------------------------------*\
| RGBController_LianLiUniversalScreen.h                     |
|                                                           |
|   RGBController for Lian Li 8.8" Universal Screen LEDs    |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      17 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "LianLiUniversalScreenController.h"
#include "RGBController.h"

class RGBController_LianLiUniversalScreen : public RGBController
{
public:
    RGBController_LianLiUniversalScreen(LianLiUniversalScreenController* controller_ptr);

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LianLiUniversalScreenController* controller;
};
