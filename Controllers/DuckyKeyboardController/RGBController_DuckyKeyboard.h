/*-----------------------------------------*\
|  RGBController_DuckyKeyboard.h            |
|                                           |
|  Generic RGB Interface for Ducky RGB      |
|  keyboard devices                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "DuckyKeyboardController.h"

class RGBController_DuckyKeyboard : public RGBController
{
public:
    RGBController_DuckyKeyboard(DuckyKeyboardController* controller_ptr);
    ~RGBController_DuckyKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    DuckyKeyboardController* controller;
};