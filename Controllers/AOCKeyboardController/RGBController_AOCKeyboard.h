/*-----------------------------------------*\
|  RGBController_AOCKeyboard.h              |
|                                           |
|  Generic RGB Interface for AOC keyboard   |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/10/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AOCKeyboardController.h"

class RGBController_AOCKeyboard : public RGBController
{
public:
    RGBController_AOCKeyboard(AOCKeyboardController* controller_ptr);
    ~RGBController_AOCKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCKeyboardController*  controller;
};
