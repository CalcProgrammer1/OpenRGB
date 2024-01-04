/*-----------------------------------------*\
|  RGBController_ValkyrieKeyboard.h         |
|                                           |
|  Generic RGB Interface for Valkyrie RGB   |
|  keyboard devices                         |
|                                           |
|  Nollie(Nuonuo)               2023/12/6   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ValkyrieKeyboardController.h"

class RGBController_ValkyrieKeyboard : public RGBController
{
public:
    RGBController_ValkyrieKeyboard(ValkyrieKeyboardController* controller_ptr);
    ~RGBController_ValkyrieKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    ValkyrieKeyboardController* controller;
};
