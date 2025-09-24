/*---------------------------------------------------------*\
| RGBController_CorsairPeripheral.h                         |
|                                                           |
|   RGBController for Corsair peripherals                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairPeripheralController.h"

class RGBController_CorsairPeripheral : public RGBController
{
public:
    RGBController_CorsairPeripheral(CorsairPeripheralController* controller_ptr, bool supports_hardware_modes);
    ~RGBController_CorsairPeripheral();

    int         physical_layout;
    int         logical_layout;

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairPeripheralController* controller;
};
