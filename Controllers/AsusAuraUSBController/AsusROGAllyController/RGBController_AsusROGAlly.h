/*---------------------------------------------------------*\
| RGBController_AsusROGAlly.h                               |
|                                                           |
|   RGBController for ASUS ROG Ally                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusROGAllyController.h"

class RGBController_AsusROGAlly : public RGBController
{
public:
    RGBController_AsusROGAlly(ROGAllyController* controller_ptr);
    ~RGBController_AsusROGAlly();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ROGAllyController* controller;
};
