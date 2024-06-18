/*---------------------------------------------------------*\
| RGBController_SonyDS4.h                                   |
|                                                           |
|   RGBController for Sony Dualshock 4                      |
|                                                           |
|   Pol Rius (alpemwarrior)                     24 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SonyDS4Controller.h"

class RGBController_SonyDS4 : public RGBController
{
public:
    RGBController_SonyDS4(SonyDS4Controller* controller_ptr);
    ~RGBController_SonyDS4();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SonyDS4Controller* controller;
};
