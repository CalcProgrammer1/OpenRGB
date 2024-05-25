/*---------------------------------------------------------*\
| RGBController_InstantMouse.h                              |
|                                                           |
|   RGBController for Instant mouse                         |
|                                                           |
|   Morgan Guimard (morg)                       19 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    InstantMouseController* controller;
};
