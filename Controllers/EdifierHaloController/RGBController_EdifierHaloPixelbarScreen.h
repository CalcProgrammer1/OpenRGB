/*---------------------------------------------------------*\
| RGBController_EdifierHaloPixelbarScreen.h                 |
|                                                           |
|   RGBController for Edifier Halo PixelBar Screen Color    |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <memory>
#include "RGBController.h"
#include "EdifierHaloController.h"

class RGBController_EdifierHaloPixelbarScreen : public RGBController
{
public:
    RGBController_EdifierHaloPixelbarScreen(EdifierHaloController* controller_ptr);
    ~RGBController_EdifierHaloPixelbarScreen();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    EdifierHaloController* controller;
    bool first_leds_update_ignored;
    bool first_mode_update_ignored;
};
