/*---------------------------------------------------------*\
| RGBController_CreativeSoundBlasterXG6.h                   |
|                                                           |
|   RGBController for Creative SoundBlaster XG6             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CreativeSoundBlasterXG6Controller.h"

#define XG6_BRIGHTNESS_MIN 0x00
#define XG6_BRIGHTNESS_MAX 0xFF

class RGBController_CreativeSoundBlasterXG6: public RGBController
{
public:
    RGBController_CreativeSoundBlasterXG6(CreativeSoundBlasterXG6Controller* controller_ptr);
    ~RGBController_CreativeSoundBlasterXG6();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CreativeSoundBlasterXG6Controller*  controller;
};
