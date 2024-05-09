/*---------------------------------------------------------*\
| RGBController_AsusAuraHeadsetStand.h                      |
|                                                           |
|   RGBController for ASUS Aura headset stand               |
|                                                           |
|   Mola19                                      06 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraHeadsetStandController.h"

enum
{
    AURA_HEADSETSTAND_BRIGHTNESS_MIN     = 0,
    AURA_HEADSETSTAND_BRIGHTNESS_MAX     = 4,
    AURA_HEADSETSTAND_BRIGHTNESS_DEFAULT = 4,
    AURA_HEADSETSTAND_SPEED_MIN          = 0,
    AURA_HEADSETSTAND_SPEED_MAX          = 255,
    AURA_HEADSETSTAND_SPEED_DEFAULT      = 127,
};

class RGBController_AuraHeadsetStand : public RGBController
{
public:
    RGBController_AuraHeadsetStand(AuraHeadsetStandController* controller_ptr);
    ~RGBController_AuraHeadsetStand();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraHeadsetStandController*  controller;
};
