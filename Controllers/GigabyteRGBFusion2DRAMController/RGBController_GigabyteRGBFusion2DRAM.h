/*-----------------------------------------*\
|  RGBController_GigabyteRGBFusion2DRAM.h   |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2 DRAM Driver        |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/7/2020    |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2DRAMController.h"

class RGBController_RGBFusion2DRAM : public RGBController
{
public:
    RGBController_RGBFusion2DRAM(RGBFusion2DRAMController* controller_ptr);
    ~RGBController_RGBFusion2DRAM();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RGBFusion2DRAMController* controller;
};
