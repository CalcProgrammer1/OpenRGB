/*---------------------------------------------------------*\
| RGBController_LogitechG915.h                              |
|                                                           |
|   RGBController for Logitech G915                         |
|                                                           |
|   Cheerpipe                                   20 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG915Controller.h"

class RGBController_LogitechG915 : public RGBController
{
public:
    RGBController_LogitechG915(LogitechG915Controller* controller_ptr, bool tkl);
    ~RGBController_LogitechG915();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    bool is_tkl;

    LogitechG915Controller* controller;
    std::vector<RGBColor>   current_colors;
};
