/*---------------------------------------------------------*\
| RGBController_SkyloongGK68HEPro.h                         |
|                                                           |
|   RGBController for Skyloong GK68HE Pro keyboard          |
|                                                           |
|   Titan                                       05 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoyuanKeyboardController.h"

#define GK68HE_PRO_LED_COUNT                67

class RGBController_SkyloongGK68HEPro : public RGBController
{
public:
    RGBController_SkyloongGK68HEPro(RoyuanKeyboardController* controller_ptr);
    ~RGBController_SkyloongGK68HEPro();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoyuanKeyboardController* controller;

    void SendCustomLayer();
    bool LayerColorsChanged();

    unsigned char flashed_colors[GK68HE_PRO_LED_COUNT * 3];
    bool          layer_flashed = false;

    std::vector<unsigned char> mode_options;
};
