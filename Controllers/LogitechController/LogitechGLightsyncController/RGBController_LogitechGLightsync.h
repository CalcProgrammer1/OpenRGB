/*---------------------------------------------------------*\
| RGBController_LogitechGLightsync.h                        |
|                                                           |
|   RGBController for Logitech Lightsync                    |
|                                                           |
|   TheRogueZeta                                21 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechGLightsyncController.h"

class RGBController_LogitechGLightsync : public RGBController
{
public:
    RGBController_LogitechGLightsync(LogitechGLightsyncController* controller_ptr);
    ~RGBController_LogitechGLightsync();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGLightsyncController* controller;
};
