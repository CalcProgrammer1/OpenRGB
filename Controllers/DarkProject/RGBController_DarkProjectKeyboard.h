/*---------------------------------------------------------*\
| RGBController_DarkProjectKeyboard.h                       |
|                                                           |
|   RGBController for Dark Project keyboard                 |
|                                                           |
|   Chris M (DrNo)                              08 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "DarkProjectKeyboardController.h"

class RGBController_DarkProjectKeyboard : public RGBController
{
public:
    RGBController_DarkProjectKeyboard(DarkProjectKeyboardController* controller_ptr);
    ~RGBController_DarkProjectKeyboard();

    void    SetupZones();
    void    ResizeZone(int zone, int new_size);

    void    DeviceUpdateLEDs();
    void    UpdateZoneLEDs(int zone);
    void    UpdateSingleLED(int led);

    void    DeviceUpdateMode();

private:
    DarkProjectKeyboardController* controller;
};
