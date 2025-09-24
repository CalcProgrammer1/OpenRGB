/*---------------------------------------------------------*\
| RGBController_InstantMouse.h                              |
|                                                           |
|   RGBController for Instant mouse                         |
|                                                           |
|   Morgan Guimard (morg)                       19 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "InstantMouseController.h"

class RGBController_InstantMouse : public RGBController
{
public:
    RGBController_InstantMouse(InstantMouseController* controller_ptr);
    ~RGBController_InstantMouse();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    InstantMouseController* controller;
};
