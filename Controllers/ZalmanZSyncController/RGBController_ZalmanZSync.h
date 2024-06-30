/*---------------------------------------------------------*\
| RGBController_ZalmanZSync.h                               |
|                                                           |
|   RGBController for Zalman Z Sync                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZalmanZSyncController.h"

class RGBController_ZalmanZSync : public RGBController
{
public:
    RGBController_ZalmanZSync(ZalmanZSyncController* controller_ptr);
    ~RGBController_ZalmanZSync();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ZalmanZSyncController*      controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
