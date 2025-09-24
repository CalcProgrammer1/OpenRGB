/*---------------------------------------------------------*\
| RGBController_RoccatKoneAimo.h                            |
|                                                           |
|   RGBController for Roccat Kone Aimo                      |
|                                                           |
|   Thibaud M (enlight3d)                       17 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatKoneAimoController.h"

class RGBController_RoccatKoneAimo : public RGBController
{
public:
    RGBController_RoccatKoneAimo(RoccatKoneAimoController* controller_ptr);
    ~RGBController_RoccatKoneAimo();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatKoneAimoController*                   controller;
    std::vector<ROCCAT_KONE_AIMO_CHANNEL>       zones_channel;
    std::vector<ROCCAT_KONE_AIMO_CHANNEL>       leds_channel;
};
