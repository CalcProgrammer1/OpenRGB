/*---------------------------------------------------------*\
| RGBController_AlienwareKeyboard.h                         |
|                                                           |
|   RGBController for Alienware keyboards                   |
|                                                           |
|   Shared controller for AW410K and AW510K                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AlienwareKeyboardController.h"
#include "AlienwareDevices.h"

class RGBController_AlienwareKeyboard : public RGBController
{
public:
    RGBController_AlienwareKeyboard(AlienwareKeyboardController* controller_ptr, const alienware_device* device_ptr);
    ~RGBController_AlienwareKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AlienwareKeyboardController*    controller;
    const alienware_device*         device;
    std::vector<RGBColor>           current_colors;
};
