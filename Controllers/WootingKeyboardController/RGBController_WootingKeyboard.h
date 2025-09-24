/*---------------------------------------------------------*\
| RGBController_WootingKeyboard.h                           |
|                                                           |
|   RGBController for Wooting keyboard                      |
|                                                           |
|   Diogo Trindade (diogotr7)                   25 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "WootingKeyboardController.h"

class RGBController_WootingKeyboard : public RGBController
{
public:
    RGBController_WootingKeyboard(WootingKeyboardController* controller_ptr);
    ~RGBController_WootingKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    WootingKeyboardController* controller;
};
