/*---------------------------------------------------------*\
| RGBController_LexipMouse.h                                |
|                                                           |
|   RGBController for Lexip mouse                           |
|                                                           |
|   Morgan Guimard (morg)                       21 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LexipMouseController* controller;
};
