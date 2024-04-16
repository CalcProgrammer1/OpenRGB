/*-----------------------------------------*\
|  RGBController_NZXTMouse.h                |
|                                           |
|  Generic RGB Interface for NZXT Mouse     |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/16/2023  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NZXTMouseController.h"

class RGBController_NZXTMouse : public RGBController
{
public:
    RGBController_NZXTMouse(NZXTMouseController* controller_ptr);
    ~RGBController_NZXTMouse();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NZXTMouseController*        controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
