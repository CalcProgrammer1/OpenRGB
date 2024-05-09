/*---------------------------------------------------------*\
| RGBController_AsusROGStrixEvolve.h                        |
|                                                           |
|   RGBController for ASUS ROG Evolve                       |
|                                                           |
|   Mola19                                      30 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AsusAuraMouseGen1Controller*  controller;
};
