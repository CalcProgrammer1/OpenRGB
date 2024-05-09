/*---------------------------------------------------------*\
| RGBController_AsusAuraKeyboard.h                          |
|                                                           |
|   RGBController for ASUS Aura keyboard                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraKeyboardController.h"

enum AuraKeyboardMappingLayoutType
{
    FLARE_LAYOUT,
    SCOPE_LAYOUT,
    SCOPE_RX_LAYOUT,
    SCOPE_TKL_LAYOUT,
    FALCHION_LAYOUT,
};

typedef struct
{
    const char*         name;
    unsigned char       idx;
} aura_keyboard_led;

typedef struct
{
    const char*         name;
    const zone_type     type;
    const unsigned int  size;
    matrix_map_type*    matrix;
} led_zone;

class RGBController_AuraKeyboard : public RGBController
{
public:
    RGBController_AuraKeyboard(AuraKeyboardController* controller_ptr, AuraKeyboardMappingLayoutType keyboard_layout);
    ~RGBController_AuraKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AuraKeyboardController*         controller;
    AuraKeyboardMappingLayoutType   layout;
};
