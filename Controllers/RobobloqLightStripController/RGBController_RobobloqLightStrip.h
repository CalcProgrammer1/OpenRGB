/*---------------------------------------------------------*\
| RGBController_RobobloqLightStrip.h                        |
|                                                           |
|   Detector for Robobloq Monitor Light Strip               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RobobloqLightStripController.h"

class RGBController_RobobloqLightStrip : public RGBController
{
public:
    RGBController_RobobloqLightStrip(RobobloqLightStripController* controller_ptr);
    ~RGBController_RobobloqLightStrip();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    /*-----------------------------------------------------*\
    | Last mode set via this controller                     |
    \*-----------------------------------------------------*/
    int         cur_mode = -1;
    RobobloqLightStripController* controller;
};
