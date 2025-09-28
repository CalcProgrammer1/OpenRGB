/*---------------------------------------------------------*\
| RGBController_LogitechG203L.h                             |
|                                                           |
|   Driver for Logitech G203L                               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG203LController.h"

class RGBController_LogitechG203L : public RGBController
{
public:
    RGBController_LogitechG203L(LogitechG203LController* controller_ptr);
    ~RGBController_LogitechG203L();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG203LController* controller;
};
