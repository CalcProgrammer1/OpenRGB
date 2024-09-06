/*---------------------------------------------------------*\
| RGBController_Luxafor.h                                   |
|                                                           |
|   RGBController for Luxafor devices                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "LuxaforController.h"
#include "RGBController.h"

/*---------------------------------------------------------*\
| Additional "pseudo-modes" which combine pattern mode with |
| the pattern to use.                                       |
\*---------------------------------------------------------*/
enum
{
    LUXAFOR_MODE_PATTERN_TRAFFIC_LIGHTS = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_TRAFFIC_LIGHTS << 8),
    LUXAFOR_MODE_PATTERN_2              = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_2 << 8),
    LUXAFOR_MODE_PATTERN_3              = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_3 << 8),
    LUXAFOR_MODE_PATTERN_4              = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_4 << 8),
    LUXAFOR_MODE_PATTERN_POLICE         = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_POLICE << 8),
    LUXAFOR_MODE_PATTERN_6              = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_6 << 8),
    LUXAFOR_MODE_PATTERN_7              = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_7 << 8),
    LUXAFOR_MODE_PATTERN_8              = LUXAFOR_MODE_PATTERN + (LUXAFOR_PATTERN_8 << 8),
};

class RGBController_Luxafor : public RGBController
{
public:
    RGBController_Luxafor(LuxaforController* controller_ptr);
    ~RGBController_Luxafor();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    LuxaforController* controller;
};
