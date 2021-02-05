/*-----------------------------------------*\
|  RGBController_HyperXDRAM.h               |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  HyperX Predator and Fury RGB interface   |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HyperXDRAMController.h"

class RGBController_HyperXDRAM : public RGBController
{
public:
    RGBController_HyperXDRAM(HyperXDRAMController* hyperx_ptr);
    ~RGBController_HyperXDRAM();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    HyperXDRAMController* hyperx;
};
