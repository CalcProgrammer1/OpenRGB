/*---------------------------------------------------------*\
| RGBController_LogitechG522.h                              |
|                                                           |
|   RGBController for Logitech G522 headset                 |
|                                                           |
|   fawmdev                                     03 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG522Controller.h"

class RGBController_LogitechG522 : public RGBController
{
public:
    RGBController_LogitechG522(LogitechG522Controller* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG522Controller* controller;
};
