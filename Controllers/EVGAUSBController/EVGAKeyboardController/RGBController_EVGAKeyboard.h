/*---------------------------------------------------------*\
| RGBController_EVGAKeyboard.h                              |
|                                                           |
|   RGBController for EVGA keyboard                         |
|                                                           |
|   Chris M (Dr_No)                             25 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAKeyboardController.h"

class RGBController_EVGAKeyboard : public RGBController
{
public:
    RGBController_EVGAKeyboard(EVGAKeyboardController* controller_ptr);
    ~RGBController_EVGAKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    uint8_t     current_mode;

    int         GetDeviceMode();
    RGBColor    GetRandomColor();

    EVGAKeyboardController* controller;
};
