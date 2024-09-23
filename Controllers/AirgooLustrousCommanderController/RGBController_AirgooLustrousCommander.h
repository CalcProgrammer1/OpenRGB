/*---------------------------------------------------------*\
| RGBController_AirgooLustrousCommander.cpp                 |
|                                                           |
|   RGBController for Airgoo Lustrous Commander             |
|                                                           |
|   Zacahry Guinn                               07 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AirgooLustrousCommanderController.h"

class RGBController_AirgooLustrousCommander : public RGBController
{
public:
    RGBController_AirgooLustrousCommander(AirgooLustrousCommanderController* controller_ptr);
    ~RGBController_AirgooLustrousCommander();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AirgooLustrousCommanderController*  controller;
    std::vector<int>                    fanleds{0};
};
