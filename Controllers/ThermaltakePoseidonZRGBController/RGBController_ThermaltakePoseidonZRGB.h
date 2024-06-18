/*---------------------------------------------------------*\
| RGBController_ThermaltakePoseidonZRGB.h                   |
|                                                           |
|   RGBController for Thermaltake Poseidon Z RGB            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PoseidonZRGBController*   controller;
};
