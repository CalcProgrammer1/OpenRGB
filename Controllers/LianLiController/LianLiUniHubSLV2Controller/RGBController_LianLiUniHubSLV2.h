/*---------------------------------------------------------*\
| RGBController_LianLiUniHubSLV2.h                          |
|                                                           |
|   RGBController for Lian Li SLV2 Uni Hub                  |
|                                                           |
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
#include "LianLiUniHubSLV2Controller.h"
#include "RGBController.h"

class RGBController_LianLiUniHubSLV2 : public RGBController
{
public:
    RGBController_LianLiUniHubSLV2(LianLiUniHubSLV2Controller* controller_ptr);
    ~RGBController_LianLiUniHubSLV2();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LianLiUniHubSLV2Controller* controller;
    bool                    initializedMode;
};
