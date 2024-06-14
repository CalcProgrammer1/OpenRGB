/*---------------------------------------------------------*\
| RGBController_RoccatSenseAimo.h                           |
|                                                           |
|   RGBController for Roccat Sense Aimo                     |
|                                                           |
|   Mola19                                      09 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatSenseAimoController* controller;
};
