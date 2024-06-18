/*---------------------------------------------------------*\
| RGBController_RoccatVulcanKeyboard.h                      |
|                                                           |
|   RGBController for Roccat Vulcan keyboard                |
|                                                           |
|   Mola19                                      17 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatVulcanKeyboardController* controller;
    uint16_t                    pid;
};
