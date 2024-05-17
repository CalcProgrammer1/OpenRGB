/*---------------------------------------------------------*\
| RGBController_ENESMBus.h                                  |
|                                                           |
|   RGBController for ENE SMBus devices                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ENESMBusController.h"

class RGBController_ENESMBus : public RGBController
{
public:
    RGBController_ENESMBus(ENESMBusController* controller_ptr);
    ~RGBController_ENESMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ENESMBusController* controller;

    int         GetDeviceMode();
};
