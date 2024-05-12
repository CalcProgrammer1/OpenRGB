/*---------------------------------------------------------*\
| RGBController_AsusCerberusKeyboard.h                      |
|                                                           |
|   RGBController for ASUS Cerberus keyboard                |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusCerberusKeyboardController.h"

enum
{
    CERBERUS_MECH_BRIGHTNESS_MIN     = 0,
    CERBERUS_MECH_BRIGHTNESS_MAX     = 4,
    CERBERUS_MECH_BRIGHTNESS_DEFAULT = 4
};

class RGBController_AsusCerberusKeyboard : public RGBController
{
public:
    RGBController_AsusCerberusKeyboard(AsusCerberusKeyboardController* controller_ptr);
    ~RGBController_AsusCerberusKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AsusCerberusKeyboardController*  controller;
};
