/*-----------------------------------------*\
|  RGBController_ValkyrieKeyboard.h         |
|                                           |
|  Generic RGB Interface for Valkyrie RGB   |
|  keyboard devices                         |
|                                           |
|  Nollie(Nuonuo)               2023/12/6   |
|  Bartholomew Ho (imnotmental) 02/01/2024  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ValkyrieKeyboardController.h"

enum ValkyrieKeyboardMappingLayoutType
{
    NORMAL_LAYOUT,
    PRO_LAYOUT,
};

class RGBController_ValkyrieKeyboard : public RGBController
{
public:
    RGBController_ValkyrieKeyboard(ValkyrieKeyboardController* controller_ptr, ValkyrieKeyboardMappingLayoutType keyboard_layout);
    ~RGBController_ValkyrieKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    ValkyrieKeyboardController*         controller;
    ValkyrieKeyboardMappingLayoutType   layout;
};
