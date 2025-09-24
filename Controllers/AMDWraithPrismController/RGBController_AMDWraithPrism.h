/*---------------------------------------------------------*\
| RGBController_AMDWraithPrism.h                            |
|                                                           |
|   RGBController for AMD Wraith Prism                      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AMDWraithPrismController.h"

class RGBController_AMDWraithPrism : public RGBController
{
public:
    RGBController_AMDWraithPrism(AMDWraithPrismController* controller_ptr);
    ~RGBController_AMDWraithPrism();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AMDWraithPrismController*   controller;
};
