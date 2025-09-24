/*---------------------------------------------------------*\
| RGBController_DreamCheeky.h                               |
|                                                           |
|   RGBController for Dream Cheeky devices                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "DreamCheekyController.h"
#include "RGBController.h"

class RGBController_DreamCheeky : public RGBController
{
public:
    RGBController_DreamCheeky(DreamCheekyController* controller_ptr);
    ~RGBController_DreamCheeky();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    DreamCheekyController* controller;
};
