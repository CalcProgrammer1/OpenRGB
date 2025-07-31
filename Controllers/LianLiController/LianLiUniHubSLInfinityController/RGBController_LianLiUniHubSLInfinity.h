/*---------------------------------------------------------*\
| RGBController_LianLiUniHubSLInfinity.h                    |
|                                                           |
|   RGBController for Lian Li SL Infinity Uni Hub           |
|                                                           |
|   Simon McKenna                               21 Oct 2023 |
|   Will Kennedy                                17 Jan 2023 |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work.                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>
#include "LianLiUniHubSLInfinityController.h"
#include "RGBController.h"

class RGBController_LianLiUniHubSLInfinity : public RGBController
{
public:
    RGBController_LianLiUniHubSLInfinity(LianLiUniHubSLInfinityController* controller_ptr);
    ~RGBController_LianLiUniHubSLInfinity();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LianLiUniHubSLInfinityController* controller;
    bool                    initializedMode;
};
