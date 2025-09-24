/*---------------------------------------------------------*\
| RGBController_Dummy.h                                     |
|                                                           |
|   Dummy RGBController that can mimic various devices for  |
|   development and test purposes                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"

class RGBController_Dummy : public RGBController
{
public:
    RGBController_Dummy();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
};
