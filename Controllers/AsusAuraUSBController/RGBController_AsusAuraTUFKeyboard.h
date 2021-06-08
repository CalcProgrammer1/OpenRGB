/*-----------------------------------------*\
|  RGBController_AsusAuraTUFKeyboard.h      |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 03/03/2021                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraTUFKeyboardController.h"

class RGBController_AuraTUFKeyboard : public RGBController
{
public:
    RGBController_AuraTUFKeyboard(AuraTUFKeyboardController* aura_ptr);
    ~RGBController_AuraTUFKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AuraTUFKeyboardController*  aura;
};
