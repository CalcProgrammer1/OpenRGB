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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    /*-----------------------------------------------------*\
    | Last mode set via this controller                     |
    \*-----------------------------------------------------*/
    int         cur_mode = -1;
    RobobloqLightStripController* controller;
};
