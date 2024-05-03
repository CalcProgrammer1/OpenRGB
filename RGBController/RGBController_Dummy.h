/*---------------------------------------------------------*\
| RGBController_Dummy.h                                     |
|                                                           |
|   Dummy RGBController that can mimic various devices for  |
|   development and test purposes                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"

class RGBController_Dummy : public RGBController
{
public:
    RGBController_Dummy();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
};
