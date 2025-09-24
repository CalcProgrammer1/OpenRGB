/*---------------------------------------------------------*\
| RGBController_EVGAGPUv3.h                                 |
|                                                           |
|   RGBController for EVGA V3 (Ampere) GPU                  |
|                                                           |
|   TheRogueZeta                                15 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUv3Controller.h"

class RGBController_EVGAGPUv3 : public RGBController
{
public:
    RGBController_EVGAGPUv3(EVGAGPUv3Controller* controller_ptr);
    ~RGBController_EVGAGPUv3();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    EVGAGPUv3Controller* controller;
    std::vector<uint8_t> zoneIndexMap;

    uint8_t     getModeIndex(uint8_t mode_value);
};
