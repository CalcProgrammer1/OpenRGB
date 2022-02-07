/*-----------------------------------------*\
|  RGBController_AirgooLustrousCommander.h  |
|                                           |
|  Generic RGB Interface for Airgoo         |
|  Lustrous Commander                       |
|  Based on code by:                        |
|  Jeff P (@jeffp1), 2020/02/07             |
|                                           |
|  Zachary G                                |
\*-----------------------------------------*/

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
    void        SetupModes();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AirgooLustrousCommanderController*     controller;
    std::vector<int>                    fanleds{0};
};
