/*-----------------------------------------*\
|  RGBController_AsusAuraStrixEvolve.h      |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 11/30/2021                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraStrixEvolveController.h"

enum
{
    AURA_STRIX_EVOLVE_BRIGHTNESS_MIN     = 0,
    AURA_STRIX_EVOLVE_BRIGHTNESS_MAX     = 255,
    AURA_STRIX_EVOLVE_BRIGHTNESS_DEFAULT = 255
};

class RGBController_AuraStrixEvolve : public RGBController
{
public:
    RGBController_AuraStrixEvolve(AuraStrixEvolveController* controller_ptr);
    ~RGBController_AuraStrixEvolve();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraStrixEvolveController*  controller;
};
