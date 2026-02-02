/*---------------------------------------------------------*\
| RGBController_LogitechG933.h                              |
|                                                           |
|   RGBController for Logitech G933                         |
|                                                           |
|   Edbgon                                      21 Jun 2021 |
|   Based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG933Controller.h"

class RGBController_LogitechG933 : public RGBController
{
public:
    RGBController_LogitechG933(LogitechG933Controller* controller_ptr);
    ~RGBController_LogitechG933();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG933Controller* controller;
};
