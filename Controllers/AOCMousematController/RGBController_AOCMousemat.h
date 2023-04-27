/*-----------------------------------------*\
|  RGBController_AOCMousemat.h              |
|                                           |
|  Generic RGB Interface for AOC mousemat   |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/15/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AOCMousematController.h"

class RGBController_AOCMousemat : public RGBController
{
public:
    RGBController_AOCMousemat(AOCMousematController* controller_ptr);
    ~RGBController_AOCMousemat();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCMousematController*  controller;
};
