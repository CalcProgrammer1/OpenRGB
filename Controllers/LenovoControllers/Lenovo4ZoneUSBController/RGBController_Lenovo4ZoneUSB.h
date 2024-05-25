/*---------------------------------------------------------*\
| RGBController_Lenovo4ZoneUSB.h                            |
|                                                           |
|   RGBController for Lenovo 4-Zone devices                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    KeyboardState state;

    Lenovo4ZoneUSBController *controller;
};
