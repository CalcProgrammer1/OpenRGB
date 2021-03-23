/*-----------------------------------------*\
|  RGBController_EVisionKeyboard.h          |
|                                           |
|  Generic RGB Interface for EVision RGB    |
|  Keyboard                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/25/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "EVisionKeyboardController.h"

class RGBController_EVisionKeyboard : public RGBController
{
public:
    RGBController_EVisionKeyboard(EVisionKeyboardController* controller_ptr);
    ~RGBController_EVisionKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    EVisionKeyboardController*  controller;
};
