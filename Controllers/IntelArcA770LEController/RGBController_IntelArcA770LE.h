/*---------------------------------------------------------*\
| RGBController_IntelArcA770LE.h                            |
|                                                           |
|   RGBController for Intel Arc A770 LE                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "IntelArcA770LEController.h"

class RGBController_IntelArcA770LE : public RGBController
{
public:
    RGBController_IntelArcA770LE(IntelArcA770LEController* controller_ptr);
    ~RGBController_IntelArcA770LE();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    IntelArcA770LEController*   controller;
};
