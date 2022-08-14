/*-----------------------------------------*\
|  RGBController_MSIRGB.h                   |
|                                           |
|  Generic RGB Interface for MSI-RGB        |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/14/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "MSIRGBController.h"

class RGBController_MSIRGB : public RGBController
{
public:
    RGBController_MSIRGB(MSIRGBController* controller_ptr);
    ~RGBController_MSIRGB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSIRGBController*       controller;
};
