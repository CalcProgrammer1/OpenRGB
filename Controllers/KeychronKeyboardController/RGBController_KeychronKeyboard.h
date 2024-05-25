/*---------------------------------------------------------*\
| RGBController_KeychronKeyboard.h                          |
|                                                           |
|   RGBController for Keychron keyboard                     |
|                                                           |
|   Morgan Guimard (morg)                       20 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    KeychronKeyboardController* controller;
};
