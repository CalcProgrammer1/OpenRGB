/*---------------------------------------------------------*\
| RGBController_ENESMBus.h                                  |
|                                                           |
|   RGBController for ENE SMBus devices                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ENESMBusController* controller;

    int         GetDeviceMode();
};
