/*---------------------------------------------------------*\
| RGBController_LogitechGLightsync1zone.h                   |
|                                                           |
|   RGBController for single zone Logitech Lightsync        |
|                                                           |
|   TheRogueZeta                                21 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGLightsyncController* controller;
};
