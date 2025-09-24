/*---------------------------------------------------------*\
| RGBController_HyperXDRAM.h                                |
|                                                           |
|   RGBController for HyperX/Kingston Fury RAM              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                29 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HyperXDRAMController.h"

class RGBController_HyperXDRAM : public RGBController
{
public:
    RGBController_HyperXDRAM(HyperXDRAMController* controller_ptr);
    ~RGBController_HyperXDRAM();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HyperXDRAMController* controller;
};
