/*---------------------------------------------------------*\
| RGBController_HPOmen30L.h                                 |
|                                                           |
|   RGBController for HP Omen 30L                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HPOmen30LController.h"

class RGBController_HPOmen30L : public RGBController
{
public:
    RGBController_HPOmen30L(HPOmen30LController* controller_ptr);
    ~RGBController_HPOmen30L();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HPOmen30LController* controller;
};
