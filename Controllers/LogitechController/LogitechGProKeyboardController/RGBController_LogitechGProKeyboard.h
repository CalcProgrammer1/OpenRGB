/*---------------------------------------------------------*\
| RGBController_LogitechGPro.h                              |
|                                                           |
|   RGBController for Logitech G Pro keyboard               |
|                                                           |
|   sanchezzzs                                  20 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechGProKeyboardController.h"

class RGBController_LogitechGProKeyboard : public RGBController
{
public:
    RGBController_LogitechGProKeyboard(LogitechGProKeyboardController* controller_ptr);
    ~RGBController_LogitechGProKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGProKeyboardController* controller;
};
