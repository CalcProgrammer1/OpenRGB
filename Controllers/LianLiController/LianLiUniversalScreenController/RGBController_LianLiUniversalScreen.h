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
    ~RGBController_LianLiUniversalScreen();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LianLiUniversalScreenController* controller;
};
