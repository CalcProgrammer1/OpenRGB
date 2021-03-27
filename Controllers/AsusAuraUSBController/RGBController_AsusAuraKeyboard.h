/*-----------------------------------------*\
|  RGBController_AsusAuraKeyboard.h         |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/19/2021   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraKeyboardController.h"

class RGBController_AuraKeyboard : public RGBController
{
public:
    RGBController_AuraKeyboard(AuraKeyboardController* aura_ptr);
    ~RGBController_AuraKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AuraKeyboardController* aura;
};
