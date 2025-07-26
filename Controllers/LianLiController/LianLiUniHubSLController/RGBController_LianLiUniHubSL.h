/*---------------------------------------------------------*\
| RGBController_LianLiUniHubSL.h                            |
|                                                           |
|   RGBController for Lian Li Uni Hub - SL                  |
|                                                           |
|   Muhamad Visat                               26 Jul 2025 |
|   Original work by Luca Lovisa & Oliver P                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LianLiUniHubSLController.h"

class RGBController_LianLiUniHubSL : public RGBController
{
public:
    RGBController_LianLiUniHubSL(LianLiUniHubSLController *controller, std::string name);
    ~RGBController_LianLiUniHubSL();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void SetCustomMode();

private:
    LianLiUniHubSLController *controller;
    bool initialized;
};
