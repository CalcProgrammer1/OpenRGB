/*---------------------------------------------------------*\
| RGBController_BloodyB820R.h                               |
|                                                           |
|   RGBController for A4Tech Bloody B820R Keyboard          |
|                                                           |
|   Mohammed Julfikar Ali Mahbub (o-julfikar)   01 Apr 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "BloodyB820RController.h"

class RGBController_BloodyB820R : public RGBController
{
public:
    RGBController_BloodyB820R(BloodyB820RController* controller_ptr);
    ~RGBController_BloodyB820R();

    void    SetupZones();

    void    DeviceUpdateLEDs();
    void    DeviceUpdateZoneLEDs(int zone);
    void    DeviceUpdateSingleLED(int led);

    void    DeviceUpdateMode();

private:
    BloodyB820RController* controller;
};
