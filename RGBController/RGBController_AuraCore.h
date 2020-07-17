/*-----------------------------------------*\
|  RGBController_AuraCore.h                 |
|                                           |
|  Generic RGB Interface for ROG Aura Core  |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/17/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AuraCoreController.h"

class RGBController_AuraCore : public RGBController
{
public:
    RGBController_AuraCore(AuraCoreController* aura_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AuraCoreController* aura;

};
