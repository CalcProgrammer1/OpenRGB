/*-----------------------------------------*\
|  RGBController_LianLiGAIITrinity.h        |
|                                           |
|  Lian Li GAII Trinity AIO LED Driver      |
|                                           |
|  Michael Losert   2023-10-27              |
\*-----------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>

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

    void SetCustomMode();

    LianLiGAIITrinityController::GAII_Direction OpenRGBDirection2GAIIDirection(unsigned int openrgb_direction)
    {
        if(openrgb_direction == MODE_DIRECTION_LEFT)
            return LianLiGAIITrinityController::GAII_Direction::D_LEFT;

        return LianLiGAIITrinityController::GAII_Direction::D_RIGHT;
    }

private:
    LianLiGAIITrinityController* controller;
};
