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
    AURA_MOUSEMAT_BRIGHTNESS_MIN                = 0,
    AURA_MOUSEMAT_BRIGHTNESS_MAX                = 4,
    AURA_MOUSEMAT_BRIGHTNESS_DEFAULT            = 4,
    AURA_MOUSEMAT_SPEED_MIN                     = 127,
    AURA_MOUSEMAT_SPEED_MAX                     = 0,
    AURA_MOUSEMAT_SPEED_DEFAULT_STATIC          = 0,
    AURA_MOUSEMAT_SPEED_DEFAULT_BREATHING       = 1,
    AURA_MOUSEMAT_SPEED_DEFAULT_COLOR_CYCLE     = 1,
    AURA_MOUSEMAT_SPEED_DEFAULT_WAVE            = 14,
    AURA_MOUSEMAT_SPEED_DEFAULT_WAVE_PLANE      = 2,
    AURA_MOUSEMAT_SPEED_DEFAULT_COMET           = 2,
    AURA_MOUSEMAT_SPEED_DEFAULT_GLOWING_YOYO    = 4,
    AURA_MOUSEMAT_SPEED_DEFAULT_CROSS           = 12,
    AURA_MOUSEMAT_SPEED_DEFAULT_STARRY_NIGHT    = 2,
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

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraMousematController* controller;
};
