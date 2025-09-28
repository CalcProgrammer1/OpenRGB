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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGLightsyncController* controller;
};
