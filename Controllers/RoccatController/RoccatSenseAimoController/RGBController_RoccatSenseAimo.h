/*---------------------------------------------------------*\
| RGBController_RoccatSenseAimo.h                           |
|                                                           |
|   RGBController for Roccat Sense Aimo                     |
|                                                           |
|   Mola19                                      09 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatSenseAimoController.h"

class RGBController_RoccatSenseAimo : public RGBController
{
public:
    RGBController_RoccatSenseAimo(RoccatSenseAimoController* controller_ptr);
    ~RGBController_RoccatSenseAimo();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatSenseAimoController* controller;
};
