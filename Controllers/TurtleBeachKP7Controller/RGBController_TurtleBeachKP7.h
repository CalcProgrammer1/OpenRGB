/*---------------------------------------------------------*\
| RGBController_TurtleBeachKP7.h                            |
|                                                           |
|   RGBController for Turtle Beach Command Series KP7       |
|                                                           |
|   Joe Peterson (joevpeterson@gmail.com)       05 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "TurtleBeachKP7Controller.h"

enum
{
    TURTLE_BEACH_KP7_MODE_DIRECT
};

class RGBController_TurtleBeachKP7 : public RGBController
{
public:
    RGBController_TurtleBeachKP7(TurtleBeachKP7Controller* controller_ptr);
    ~RGBController_TurtleBeachKP7();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    TurtleBeachKP7Controller*   controller;
};
