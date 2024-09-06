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
