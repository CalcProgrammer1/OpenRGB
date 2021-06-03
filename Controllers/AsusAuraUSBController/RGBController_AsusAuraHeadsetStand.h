/*-----------------------------------------*\
|  RGBController_AsusAuraHeadsetStand.h     |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 06/04/2021                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraHeadsetStandController.h"

class RGBController_AuraHeadsetStand : public RGBController
{
public:
    RGBController_AuraHeadsetStand(AuraHeadsetStandController* aura_ptr);
    ~RGBController_AuraHeadsetStand();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AuraHeadsetStandController*  aura;
};
