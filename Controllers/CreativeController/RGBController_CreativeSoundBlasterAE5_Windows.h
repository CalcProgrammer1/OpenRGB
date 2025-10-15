/*---------------------------------------------------------*\
| RGBController_CreativeSoundBlasterAE5_Windows.h           |
|                                                           |
|   RGBController for Creative SoundBlaster AE-5 (Windows)  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CreativeSoundBlasterAE5ControllerBase.h"

class RGBController_CreativeSoundBlasterAE5: public RGBController
{
public:
    RGBController_CreativeSoundBlasterAE5(CreativeSoundBlasterAE5ControllerBase* controller_ptr);
    ~RGBController_CreativeSoundBlasterAE5();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CreativeSoundBlasterAE5ControllerBase*  controller;
    void        UpdateLEDRange(unsigned int start_led, unsigned int led_count);
};