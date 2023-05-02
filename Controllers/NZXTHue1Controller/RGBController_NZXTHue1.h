/*-----------------------------------------*\
|  RGBController_NZXTHue1.h                 |
|                                           |
|  Generic RGB Interface for NZXT Hue+      |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "NZXTHue1Controller.h"

class RGBController_NZXTHue1 : public RGBController
{
public:
    RGBController_NZXTHue1(NZXTHue1Controller* controller_ptr);
    ~RGBController_NZXTHue1();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NZXTHue1Controller*         controller;
};
