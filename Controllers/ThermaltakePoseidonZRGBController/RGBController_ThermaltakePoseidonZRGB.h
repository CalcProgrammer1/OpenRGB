/*-----------------------------------------*\
|  RGBController_ThermaltakePoseidonZRGB.h  |
|                                           |
|  Generic RGB Interface for Thermaltake    |
|  Poseidon Z RGB Keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ThermaltakePoseidonZRGBController.h"

class RGBController_PoseidonZRGB : public RGBController
{
public:
    RGBController_PoseidonZRGB(PoseidonZRGBController* controller_ptr);
    ~RGBController_PoseidonZRGB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PoseidonZRGBController*   controller;
};