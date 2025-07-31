/*---------------------------------------------------------*\
| RGBController_LianLiGAIITrinity.h                         |
|                                                           |
|   RGBController for Lian Li GAII Trinity                  |
|                                                           |
|   Michael Losert                              27 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "LianLiGAIITrinityController.h"
#include "RGBController.h"

class RGBController_LianLiGAIITrinity : public RGBController
{
public:
    RGBController_LianLiGAIITrinity(LianLiGAIITrinityController* controller_ptr);
    ~RGBController_LianLiGAIITrinity();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    LianLiGAIITrinityController::GAII_Direction OpenRGBDirection2GAIIDirection(unsigned int openrgb_direction)
    {
        if(openrgb_direction == MODE_DIRECTION_LEFT)
            return LianLiGAIITrinityController::GAII_Direction::D_LEFT;

        return LianLiGAIITrinityController::GAII_Direction::D_RIGHT;
    }

private:
    LianLiGAIITrinityController* controller;
};
