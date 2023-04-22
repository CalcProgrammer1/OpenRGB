/*-----------------------------------------*\
|  RGBController_AOCMouse.h                 |
|                                           |
|  Generic RGB Interface for AOC mouse      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/20/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AOCMouseController.h"

class RGBController_AOCMouse : public RGBController
{
public:
    RGBController_AOCMouse(AOCMouseController* controller_ptr);
    ~RGBController_AOCMouse();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCMouseController*  controller;
};
