/*---------------------------------------------------------*\
| RGBController_DreamCheeky.h                               |
|                                                           |
|   RGBController for Dream Cheeky devices                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    DreamCheekyController* controller;
};
