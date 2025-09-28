/*---------------------------------------------------------*\
| RGBController_Crucial.h                                   |
|                                                           |
|   RGBController for Crucial Ballistix RAM                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CrucialController.h"

class RGBController_Crucial : public RGBController
{
public:
    RGBController_Crucial(CrucialController* controller_ptr);
    ~RGBController_Crucial();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CrucialController*  controller;
};
