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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG203LController* controller;
};
