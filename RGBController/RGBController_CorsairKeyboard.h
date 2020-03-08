/*-----------------------------------------*\
|  RGBController_CorsairKeyboard.h          |
|                                           |
|  Generic RGB Interface for Corsair RGB    |
|  keyboards                                |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CorsairKeyboardController.h"

class RGBController_CorsairKeyboard : public RGBController
{
public:
    RGBController_CorsairKeyboard(CorsairKeyboardController* corsair_ptr);
    ~RGBController_CorsairKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    CorsairKeyboardController*   corsair;
};
