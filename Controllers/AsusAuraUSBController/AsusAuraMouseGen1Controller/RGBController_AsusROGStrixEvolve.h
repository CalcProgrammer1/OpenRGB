/*---------------------------------------------------------*\
| RGBController_AsusROGStrixEvolve.h                        |
|                                                           |
|   RGBController for ASUS ROG Evolve                       |
|                                                           |
|   Mola19                                      30 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraMouseGen1Controller.h"

enum
{
    ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MIN        = 0,
    ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_MAX        = 255,
    ASUS_ROG_STRIX_EVOLVE_BRIGHTNESS_DEFAULT    = 255,
};

enum
{
    ASUS_ROG_STRIX_EVOLVE_MODE_DIRECT           = 0x01,
    ASUS_ROG_STRIX_EVOLVE_MODE_BREATHING        = 0x02,
    ASUS_ROG_STRIX_EVOLVE_MODE_SPECTRUM_CYCLE   = 0x03,
    ASUS_ROG_STRIX_EVOLVE_MODE_REACTIVE         = 0x04,
};

class RGBController_AsusROGStrixEvolve : public RGBController
{
public:
    RGBController_AsusROGStrixEvolve(AsusAuraMouseGen1Controller* controller_ptr);
    ~RGBController_AsusROGStrixEvolve();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AsusAuraMouseGen1Controller*  controller;
};
