/*---------------------------------------------------------*\
| RGBController_WootingKeyboard.h                           |
|                                                           |
|   RGBController for Wooting keyboard                      |
|                                                           |
|   Diogo Trindade (diogotr7)                   04 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    WootingKeyboardController* controller;
};
