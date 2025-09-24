/*---------------------------------------------------------*\
| RGBController_KeychronKeyboard.h                          |
|                                                           |
|   RGBController for Keychron keyboard                     |
|                                                           |
|   Morgan Guimard (morg)                       20 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "KeychronKeyboardController.h"

class RGBController_KeychronKeyboard : public RGBController
{
public:
    RGBController_KeychronKeyboard(KeychronKeyboardController* controller_ptr);
    ~RGBController_KeychronKeyboard();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    KeychronKeyboardController* controller;
};
