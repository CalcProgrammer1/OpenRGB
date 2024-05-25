/*---------------------------------------------------------*\
| RGBController_LenovoM300.h                                |
|                                                           |
|   RGBController for Lenovo Legion M300 mouse              |
|                                                           |
|   Wayne Riordan                               09 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LenovoM300Controller.h"

class RGBController_LenovoM300 : public RGBController
{
public:
    RGBController_LenovoM300(LenovoM300Controller* controller_ptr);
    ~RGBController_LenovoM300();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LenovoM300Controller* controller;
};
