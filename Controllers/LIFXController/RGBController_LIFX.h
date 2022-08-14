/*-----------------------------------------*\
|  RGBController_LIFX.h                     |
|                                           |
|  Generic RGB Interface for LIFX           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/5/2022    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LIFXController.h"

class RGBController_LIFX : public RGBController
{
public:
    RGBController_LIFX(LIFXController* controller_ptr);
    ~RGBController_LIFX();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LIFXController* controller;
};
