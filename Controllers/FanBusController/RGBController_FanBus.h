/*---------------------------------------------------------*\
| RGBController_FanBus.h                                    |
|                                                           |
|   RGBController for FanBus devices                        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    FanBusController*   controller;
};
