/*---------------------------------------------------------*\
| RGBController_CherryKeyboard.h                            |
|                                                           |
|   RGBController for Cherry keyboard                       |
|                                                           |
|   Sebastian Kraus                             25 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CherryKeyboardController.h"

class RGBController_CherryKeyboard : public RGBController
{
public:
    RGBController_CherryKeyboard(CherryKeyboardController* controller_ptr, uint16_t product_id);
    ~RGBController_CherryKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CherryKeyboardController*  controller;

    static bool hasUnofficialModeSupport(uint16_t product_id);
};
