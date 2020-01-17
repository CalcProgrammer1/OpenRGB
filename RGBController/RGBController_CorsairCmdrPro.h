/*-----------------------------------------*\
|  RGBController_CorsairCmdrPro.h           |
|                                           |
|  Generic RGB Interface for Corsair        |
|  Commander Pro                            |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/16/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "serial_port.h"
#include "CorsairCmdrProController.h"

class RGBController_CorsairCmdrPro : public RGBController
{
public:
    RGBController_CorsairCmdrPro(CorsairCmdrProController* corsair_ptr);
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    CorsairCmdrProController*   corsair;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
