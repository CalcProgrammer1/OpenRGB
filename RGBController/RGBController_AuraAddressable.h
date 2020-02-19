/*-----------------------------------------*\
|  RGBController_AuraAddressable.h          |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  addressable controller driver            |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AuraAddressableController.h"

#define AURA_ADDRESSABLE_MAX_LEDS       120
#define AURA_ADDRESSABLE_NUM_CHANNELS   1

class RGBController_AuraAddressable : public RGBController
{
public:
    RGBController_AuraAddressable(AuraAddressableController* aura_ptr);
    ~RGBController_AuraAddressable();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    AuraAddressableController*  aura;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};