/*---------------------------------------------------------*\
| RGBController_LogitechX56.h                               |
|                                                           |
|   RGBController for Logitech X56                          |
|                                                           |
|   Edbgon                                      11 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechX56Controller.h"

class RGBController_LogitechX56 : public RGBController
{
public:
    RGBController_LogitechX56(LogitechX56Controller* controller_ptr);
    ~RGBController_LogitechX56();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    LogitechX56Controller*      controller;
};
