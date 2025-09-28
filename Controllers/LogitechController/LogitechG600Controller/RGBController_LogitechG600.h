/*---------------------------------------------------------*\
| RGBController_LogitechG600.h                              |
|                                                           |
|   RGBController for Logitech G600 Gaming Mouse            |
|                                                           |
|   Austin B     (austinleroy)                11 Sep 2025   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG600Controller.h"

class RGBController_LogitechG600 : public RGBController
{
public:
    RGBController_LogitechG600(LogitechG600Controller* controller_ptr);
    ~RGBController_LogitechG600();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG600Controller* controller;
};
