/*-----------------------------------------*\
|  RGBController_AsusAuraCore.h             |
|                                           |
|  Generic RGB Interface for ROG Aura Core  |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/17/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraCoreController.h"

class RGBController_AuraCore : public RGBController
{
public:
    RGBController_AuraCore(AuraCoreController* aura_ptr);
    ~RGBController_AuraCore();

    void        SetupKeyboard();
    void        SetupGA15DH();
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
