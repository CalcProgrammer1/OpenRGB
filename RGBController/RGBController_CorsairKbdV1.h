/*-----------------------------------------*\
|  RGBController_CorsairKbdV1.h             |
|                                           |
|  Generic RGB Interface for first-gen      |
|  Corsair RGB keyboard                     |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CorsairKbdV1Controller.h"

class RGBController_CorsairKbdV1 : public RGBController
{
public:
    RGBController_CorsairKbdV1(CorsairKbdV1Controller* corsair_ptr);
    ~RGBController_CorsairKbdV1();
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    CorsairKbdV1Controller*   corsair;
};