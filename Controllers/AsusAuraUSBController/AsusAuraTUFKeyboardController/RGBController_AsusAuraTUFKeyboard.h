/*---------------------------------------------------------*\
| RGBController_AsusAuraTUFKeyboard.h                       |
|                                                           |
|   RGBController for ASUS Aura TUF keyboard                |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraTUFKeyboardController.h"

enum
{
    AURA_KEYBOARD_BRIGHTNESS_MIN     = 0,
    AURA_KEYBOARD_BRIGHTNESS_MAX     = 4,
    AURA_KEYBOARD_BRIGHTNESS_DEFAULT = 4,
};

enum
{
    AURA_CLAYMORE_SPEED_MIN                     = 254,
    AURA_CLAYMORE_SPEED_MAX                     = 0,
    AURA_CLAYMORE_SPEED_DEFAULT_STATIC          = 0,
    AURA_CLAYMORE_SPEED_DEFAULT_BREATHING       = 107,
    AURA_CLAYMORE_SPEED_DEFAULT_COLOR_CYCLE     = 121,
    AURA_CLAYMORE_SPEED_DEFAULT_REACTIVE        = 56,
    AURA_CLAYMORE_SPEED_DEFAULT_WAVE            = 50,
    AURA_CLAYMORE_SPEED_DEFAULT_RIPPLE          = 108,
    AURA_CLAYMORE_SPEED_DEFAULT_STARRY_NIGHT    = 54,
    AURA_CLAYMORE_SPEED_DEFAULT_QUICKSAND       = 103
};

class RGBController_AuraTUFKeyboard : public RGBController
{
public:
    RGBController_AuraTUFKeyboard(AuraTUFKeyboardController* controller_ptr);
    ~RGBController_AuraTUFKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraTUFKeyboardController*  controller;
    uint16_t                    pid;
};
