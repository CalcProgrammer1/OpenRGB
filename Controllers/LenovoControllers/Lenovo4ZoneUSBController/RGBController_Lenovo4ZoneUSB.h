/*---------------------------------------------------------*\
| RGBController_Lenovo4ZoneUSB.h                            |
|                                                           |
|   RGBController for Lenovo 4-Zone devices                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LenovoDevices.h"
#include "Lenovo4ZoneUSBController.h"
#include "RGBController.h"

#define NA 0xFFFFFFFF

class RGBController_Lenovo4ZoneUSB : public RGBController
{
public:
    RGBController_Lenovo4ZoneUSB(Lenovo4ZoneUSBController* controller_ptr);
    ~RGBController_Lenovo4ZoneUSB();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    KeyboardState state;

    Lenovo4ZoneUSBController *controller;
};
