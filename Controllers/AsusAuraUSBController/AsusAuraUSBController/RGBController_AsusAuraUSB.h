/*-----------------------------------------*\
|  RGBController_AsusAuraUSB.h              |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraUSBController.h"

#define AURA_ADDRESSABLE_MAX_LEDS       120

class RGBController_AuraUSB : public RGBController
{
public:
    RGBController_AuraUSB(AuraUSBController* controller_ptr);
    ~RGBController_AuraUSB();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

protected:
    AuraUSBController*          controller;

private:
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
    bool                        initializedMode;
};
