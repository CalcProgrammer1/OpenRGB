/*-----------------------------------------*\
|  RGBController_RedragonMouse.h            |
|                                           |
|  Generic RGB Interface for Redragon Mouse |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/25/2020   |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include "RedragonMouseController.h"

class RGBController_RedragonMouse : public RGBController
{
public:
    RGBController_RedragonMouse(RedragonMouseController* controller_ptr);
    ~RGBController_RedragonMouse();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RedragonMouseController*   controller;
};
