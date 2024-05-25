/*---------------------------------------------------------*\
| RGBController_LexipMouse.h                                |
|                                                           |
|   RGBController for Lexip mouse                           |
|                                                           |
|   Morgan Guimard (morg)                       21 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LexipMouseController.h"

class RGBController_LexipMouse : public RGBController
{
public:
    RGBController_LexipMouse(LexipMouseController* controller_ptr);
    ~RGBController_LexipMouse();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LexipMouseController* controller;
};
