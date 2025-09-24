/*---------------------------------------------------------*\
| RGBController_LogitechLightspeed.h                        |
|                                                           |
|   RGBController for Logitech Lightspeed                   |
|                                                           |
|   TheRogueZeta                                05 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechLightspeedController.h"

class RGBController_LogitechLightspeed : public RGBController
{
public:
    RGBController_LogitechLightspeed(LogitechLightspeedController* controller_ptr);
    ~RGBController_LogitechLightspeed();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    uint16_t    pid;        //This is a workaround fix for G502 mode breathing / spectrum cycle swap
private:
    LogitechLightspeedController* controller;
};
