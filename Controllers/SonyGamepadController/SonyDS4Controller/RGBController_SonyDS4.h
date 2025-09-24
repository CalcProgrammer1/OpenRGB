/*---------------------------------------------------------*\
| RGBController_SonyDS4.h                                   |
|                                                           |
|   RGBController for Sony Dualshock 4                      |
|                                                           |
|   Pol Rius (alpemwarrior)                     24 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SonyDS4Controller* controller;
};
