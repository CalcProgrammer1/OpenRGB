/*---------------------------------------------------------*\
| RGBController_Seagate.h                                   |
|                                                           |
|   RGBController for Seagate                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SeagateController.h"

class RGBController_Seagate : public RGBController
{
public:
    RGBController_Seagate(SeagateController* controller_ptr);
    ~RGBController_Seagate();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        DeviceSaveMode();

private:
    SeagateController* controller;
};
