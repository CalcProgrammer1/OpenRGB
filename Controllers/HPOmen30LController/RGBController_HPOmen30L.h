/*---------------------------------------------------------*\
| RGBController_HPOmen30L.h                                 |
|                                                           |
|   RGBController for HP Omen 30L                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HPOmen30LController* controller;
};
