/*---------------------------------------------------------*\
| RGBController_LianLiUniHub.h                              |
|                                                           |
|   RGBController for Lian Li Uni Hub                       |
|                                                           |
|   Luca Lovisa                                 20 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>
#include "LianLiUniHubController.h"
#include "RGBController.h"

class RGBController_LianLiUniHub : public RGBController
{
public:
    RGBController_LianLiUniHub(LianLiUniHubController* controller_ptr);

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    uint8_t convertAnyFanCount(uint8_t count);
    uint8_t convertLedSpeed(uint8_t speed);
    uint8_t convertLedDirection(uint8_t direction);

    uint8_t convertLedCountToFanCount(uint8_t count);

private:
    LianLiUniHubController* controller;
    bool                    initializedMode;
};
