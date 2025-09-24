/*---------------------------------------------------------*\
| RGBController_Tecknet.h                                   |
|                                                           |
|   RGBController for Tecknet devices                       |
|                                                           |
|   Chris M (Dr_No)                             29 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    TecknetController* controller;
};
