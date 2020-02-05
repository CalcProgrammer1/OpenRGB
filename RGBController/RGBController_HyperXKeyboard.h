/*-----------------------------------------*\
|  RGBController_HyperXKeyboard.h           |
|                                           |
|  Generic RGB Interface for HyperX RGB     |
|  Keyboard                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "HyperXKeyboardController.h"

class RGBController_HyperXKeyboard : public RGBController
{
public:
    RGBController_HyperXKeyboard(HyperXKeyboardController* hyperx_ptr);
    ~RGBController_HyperXKeyboard();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    HyperXKeyboardController*   hyperx;
};
