/*-----------------------------------------*\
|  RGBController_CorsairNodePro.h           |
|                                           |
|  Generic RGB Interface for Corsair        |
|  Lighting Node Pro                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/12/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CorsairNodeProController.h"

class RGBController_CorsairNodePro : public RGBController
{
public:
    RGBController_CorsairNodePro(CorsairNodeProController* corsair_ptr);
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    CorsairNodeProController*   corsair;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
