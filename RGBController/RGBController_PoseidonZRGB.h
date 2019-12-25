/*-----------------------------------------*\
|  RGBController_PoseidonZRGB.h             |
|                                           |
|  Generic RGB Interface for Thermaltake    |
|  Poseidon Z RGB Keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "PoseidonZRGBController.h"

class RGBController_PoseidonZRGB : public RGBController
{
public:
    RGBController_PoseidonZRGB(PoseidonZRGBController* poseidon_ptr);
    ~RGBController_PoseidonZRGB();
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);
    void UpdateLEDs();

private:
    PoseidonZRGBController*   poseidon;
};