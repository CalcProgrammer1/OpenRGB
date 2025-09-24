/*---------------------------------------------------------*\
| RGBController_LogitechG815.h                              |
|                                                           |
|   RGBController for Logitech G815                         |
|                                                           |
|   Cheerpipe                                   20 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG815Controller.h"

class RGBController_LogitechG815 : public RGBController
{
public:
    RGBController_LogitechG815(LogitechG815Controller* controller_ptr);
    ~RGBController_LogitechG815();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG815Controller* controller;
    std::vector<RGBColor>   current_colors;
};
