/*---------------------------------------------------------*\
| RGBController_Tecknet.h                                   |
|                                                           |
|   RGBController for Tecknet devices                       |
|                                                           |
|   Chris M (Dr_No)                             29 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "TecknetController.h"

class RGBController_Tecknet : public RGBController
{
public:
    RGBController_Tecknet(TecknetController* controller_ptr);
    ~RGBController_Tecknet();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    TecknetController* controller;
};
