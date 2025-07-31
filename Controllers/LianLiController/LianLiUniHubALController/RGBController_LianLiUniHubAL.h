/*---------------------------------------------------------*\
| RGBController_LianLiUniHub_AL.h                           |
|                                                           |
|   RGBController for Lian Li AL Uni Hub                    |
|                                                           |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>
#include "LianLiUniHubALController.h"
#include "RGBController.h"

class RGBController_LianLiUniHubAL : public RGBController
{
public:
    RGBController_LianLiUniHubAL(LianLiUniHubALController* controller_ptr);
    ~RGBController_LianLiUniHubAL();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LianLiUniHubALController* controller;
    bool                    initializedMode;
};
