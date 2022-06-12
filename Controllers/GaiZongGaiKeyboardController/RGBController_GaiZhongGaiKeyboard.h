/*-----------------------------------------*\
|  RGBController_GaiZhongGaiKeyboard.h            |
|                                           |
|  Generic RGB Interface for GaiZhongGai RGB      |
|  keyboard devices                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "GaiZhongGaiKeyboardController.h"

class RGBController_GaiZhongGaiKeyboard : public RGBController
{
public:
    RGBController_GaiZhongGaiKeyboard(GaiZhongGaiKeyboardController* GaiZhongGai_ptr);
    ~RGBController_GaiZhongGaiKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    GaiZhongGaiKeyboardController*    GaiZhongGai;
};
