/*-----------------------------------------*\
|  RGBController_KeychronKeyboard.h         |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Keychron Keyboard RGB USB Driver         |
|                                           |
|  Guimard Morgan (morg) 2/20/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "KeychronKeyboardController.h"

class RGBController_KeychronKeyboard : public RGBController
{
public:
    RGBController_KeychronKeyboard(KeychronKeyboardController* controller_ptr);
    ~RGBController_KeychronKeyboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    KeychronKeyboardController* controller;
};
