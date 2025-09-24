/*---------------------------------------------------------*\
| RGBController_RoccatVulcanKeyboard.h                      |
|                                                           |
|   RGBController for Roccat Vulcan keyboard                |
|                                                           |
|   Mola19                                      17 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatVulcanKeyboardController.h"

class RGBController_RoccatVulcanKeyboard : public RGBController
{
public:
    RGBController_RoccatVulcanKeyboard(RoccatVulcanKeyboardController* controller_ptr);
    ~RGBController_RoccatVulcanKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatVulcanKeyboardController* controller;
    uint16_t                    pid;
};
