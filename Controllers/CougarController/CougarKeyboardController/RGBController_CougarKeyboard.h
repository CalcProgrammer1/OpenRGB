/*---------------------------------------------------------*\
| RGBController_CougarKeyboard.h                            |
|                                                           |
|   RGBController for Cougar keyboard                       |
|                                                           |
|   Chris M (DrNo)                              05 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "CougarKeyboardController.h"

class RGBController_CougarKeyboard : public RGBController
{
public:
    RGBController_CougarKeyboard(CougarKeyboardController* controller_ptr);
    ~RGBController_CougarKeyboard();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();
    void                DeviceSaveMode();

private:
    int                 GetDeviceMode();
    int                 GetLED_Zone(int led_idx);

    CougarKeyboardController*      controller;
};
