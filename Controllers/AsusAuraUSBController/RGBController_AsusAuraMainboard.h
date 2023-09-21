/*-----------------------------------------*\
|  RGBController_AsusAuraMainboard.h        |
|                                           |
|  RGB controller for Asus Aura USB         |
|  mainboards                               |
|                                           |
|  rytypete 8/30/2023                       |
|  based on RGBController_AsusAuraUSB.h     |
|  code                                     |
\*-----------------------------------------*/

#pragma once
#include "RGBController_AsusAuraUSB.h"
#include "AsusAuraMainboardController.h"

class RGBController_AuraMainboard : public RGBController_AuraUSB
{
public:
    RGBController_AuraMainboard(AuraMainboardController* controller_ptr);

    void        DeviceSaveMode();

private:
    void        DeviceUpdateShutdownEffect();
};
