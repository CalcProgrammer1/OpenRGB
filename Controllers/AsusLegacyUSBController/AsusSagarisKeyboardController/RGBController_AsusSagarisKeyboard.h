/*---------------------------------------------------------*\
| RGBController_AsusSagarisKeyboard.h                       |
|                                                           |
|   RGBController for ASUS Sagaris keyboard                 |
|                                                           |
|   Mola19                                      20 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusSagarisKeyboardController.h"

class RGBController_AsusSagarisKeyboard : public RGBController
{
public:
    RGBController_AsusSagarisKeyboard(AsusSagarisKeyboardController* controller_ptr);
    ~RGBController_AsusSagarisKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AsusSagarisKeyboardController*  controller;
    std::vector<RGBColor>           current_colors;
    uint8_t                         last_mode;
};
