/*---------------------------------------------------------*\
| RGBController_LogitechG933.h                              |
|                                                           |
|   RGBController for Logitech G933                         |
|                                                           |
|   Edbgon                                      21 Jun 2021 |
|   Based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG933Controller.h"

class RGBController_LogitechG933 : public RGBController
{
public:
    RGBController_LogitechG933(LogitechG933Controller* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG933Controller* controller;
};
