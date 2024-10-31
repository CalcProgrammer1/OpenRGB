/*---------------------------------------------------------*\
| RGBController_HYTEKeyboard.h                              |
|                                                           |
|   RGBController for HYTE keyboard                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      30 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "HYTEKeyboardController.h"
#include "RGBController.h"

class RGBController_HYTEKeyboard : public RGBController
{
public:
    RGBController_HYTEKeyboard(HYTEKeyboardController* controller_ptr);
    ~RGBController_HYTEKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HYTEKeyboardController*     controller;
};
