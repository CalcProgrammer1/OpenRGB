/*---------------------------------------------------------*\
| RGBController_ThermaltakePoseidonZRGB.h                   |
|                                                           |
|   RGBController for Thermaltake Poseidon Z RGB            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ThermaltakePoseidonZRGBController.h"

class RGBController_PoseidonZRGB : public RGBController
{
public:
    RGBController_PoseidonZRGB(PoseidonZRGBController* controller_ptr);
    ~RGBController_PoseidonZRGB();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PoseidonZRGBController*   controller;
};
