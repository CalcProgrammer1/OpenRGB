/*---------------------------------------------------------*\
| RGBController_FanBus.h                                    |
|                                                           |
|   RGBController for FanBus devices                        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "FanBusController.h"

class RGBController_FanBus : public RGBController
{
public:
    RGBController_FanBus(FanBusController* controller_ptr);
    ~RGBController_FanBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    FanBusController*   controller;
};
