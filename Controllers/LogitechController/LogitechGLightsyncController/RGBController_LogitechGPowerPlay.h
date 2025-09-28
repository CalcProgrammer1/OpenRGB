/*---------------------------------------------------------*\
| RGBController_LogitechGPowerPlay.h                        |
|                                                           |
|   RGBController for Logitech G PowerPlay                  |
|                                                           |
|   TheRogueZeta                                31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechGLightsyncController.h"

class RGBController_LogitechGPowerPlay : public RGBController
{
public:
    RGBController_LogitechGPowerPlay(LogitechGLightsyncController* controller_ptr);
    ~RGBController_LogitechGPowerPlay();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGLightsyncController* controller;
};
