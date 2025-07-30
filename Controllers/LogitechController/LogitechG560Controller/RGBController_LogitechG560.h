/*---------------------------------------------------------*\
| RGBController_LogitechG560.h                              |
|                                                           |
|   RGBController for Logitech G560                         |
|                                                           |
|   Cheerpipe                                   28 Oct 2020 |
|   based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG560Controller.h"

class RGBController_LogitechG560 : public RGBController
{
public:
    RGBController_LogitechG560(LogitechG560Controller* controller_ptr);

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG560Controller* controller;
};
