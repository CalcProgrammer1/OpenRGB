/*---------------------------------------------------------*\
| RGBController_AsusROGSpatha.h                             |
|                                                           |
|   RGBController for ASUS ROG Spatha                       |
|                                                           |
|   Mola19                                      05 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraMouseGen1Controller.h"

enum
{
    ASUS_ROG_SPATHA_BRIGHTNESS_MIN          = 0,
    ASUS_ROG_SPATHA_BRIGHTNESS_MAX          = 15,
    ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT      = 15
};

enum
{
    ASUS_ROG_SPATHA_MODE_DIRECT             = 0xFF,
    ASUS_ROG_SPATHA_MODE_STATIC             = 0x01,
    ASUS_ROG_SPATHA_MODE_SPECTRUM_CYCLE     = 0x05,
    ASUS_ROG_SPATHA_MODE_RANDOM             = 0x06,
    ASUS_ROG_SPATHA_MODE_BREATHING          = 0x0A,
    ASUS_ROG_SPATHA_MODE_BATTERY            = 0x0B,
    ASUS_ROG_SPATHA_MODE_REACTIVE           = 0x0C,
};

class RGBController_AsusROGSpatha : public RGBController
{
public:
    RGBController_AsusROGSpatha(AsusAuraMouseGen1Controller* controller_ptr);
    ~RGBController_AsusROGSpatha();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AsusAuraMouseGen1Controller*  controller;
};
