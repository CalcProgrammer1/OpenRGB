/*-----------------------------------------*\
|  RGBController_IntelArcA770LE.h           |
|                                           |
|  Generic RGB Interface for Intel Arc A770 |
|  LE                                       |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/2/2022   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "IntelArcA770LEController.h"

class RGBController_IntelArcA770LE : public RGBController
{
public:
    RGBController_IntelArcA770LE(IntelArcA770LEController* controller_ptr);
    ~RGBController_IntelArcA770LE();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    IntelArcA770LEController*   controller;
};
