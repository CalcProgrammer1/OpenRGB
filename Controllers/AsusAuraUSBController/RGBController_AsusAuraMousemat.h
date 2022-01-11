/*-----------------------------------------*\
|  RGBController_AsusAuraMousemat.h         |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 06/04/2021                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraMousematController.h"

enum
{
    AURA_MOUSEMAT_BRIGHTNESS_MIN        = 0,
    AURA_MOUSEMAT_BRIGHTNESS_MAX        = 4,
    AURA_MOUSEMAT_BRIGHTNESS_DEFAULT    = 4,
    AURA_MOUSEMAT_SPEED_MIN             = 0,
    AURA_MOUSEMAT_SPEED_MAX             = 255,
    AURA_MOUSEMAT_SPEED_DEFAULT         = 127,
};

class RGBController_AuraMousemat : public RGBController
{
public:
    RGBController_AuraMousemat(AuraMousematController* controller_ptr);
    ~RGBController_AuraMousemat();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraMousematController* controller;
};
