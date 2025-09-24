/*---------------------------------------------------------*\
| RGBController_LogitechGLightsync1zone.h                   |
|                                                           |
|   RGBController for single zone Logitech Lightsync        |
|                                                           |
|   TheRogueZeta                                21 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechGLightsyncController.h"

class RGBController_LogitechGLightsync1zone : public RGBController
{
public:
    RGBController_LogitechGLightsync1zone(LogitechGLightsyncController* controller_ptr);
    ~RGBController_LogitechGLightsync1zone();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGLightsyncController* controller;
};
