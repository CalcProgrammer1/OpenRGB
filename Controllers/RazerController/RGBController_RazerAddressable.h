/*-----------------------------------------*\
|  RGBController_RazerAddressable.h         |
|                                           |
|  Generic RGB Interface for Razer devices  |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/11/2021   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RazerController.h"

enum
{
    RAZER_ADDRESSABLE_MODE_DIRECT,
    RAZER_ADDRESSABLE_MODE_OFF,
    RAZER_ADDRESSABLE_MODE_STATIC,
    RAZER_ADDRESSABLE_MODE_BREATHING,
    RAZER_ADDRESSABLE_MODE_SPECTRUM_CYCLE,
    RAZER_ADDRESSABLE_MODE_WAVE,
    RAZER_ADDRESSABLE_MODE_REACTIVE,
};

class RGBController_RazerAddressable : public RGBController
{
public:
    RGBController_RazerAddressable(RazerController* controller_ptr);
    ~RGBController_RazerAddressable();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RazerController*    controller;
};
