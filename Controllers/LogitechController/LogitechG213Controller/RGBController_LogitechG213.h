/*---------------------------------------------------------*\
| RGBController_LogitechG213.h                              |
|                                                           |
|   RGBController for Logitech G203L                        |
|                                                           |
|   Eric Samuelson (edbgon)                     06 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG213Controller.h"

class RGBController_LogitechG213 : public RGBController
{
public:
    RGBController_LogitechG213(LogitechG213Controller* controller_ptr);
    ~RGBController_LogitechG213();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG213Controller* controller;
};
