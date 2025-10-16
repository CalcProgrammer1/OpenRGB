/*---------------------------------------------------------*\
| RGBController_AsusCerberusKeyboard.h                      |
|                                                           |
|   RGBController for ASUS Cerberus keyboard                |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AsusCerberusKeyboardController*  controller;
};
