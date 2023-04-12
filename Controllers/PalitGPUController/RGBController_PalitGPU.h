/*-----------------------------------------*\
|  RGBController_PalitGPU.h                 |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Palit GPU RGB Driver                     |
|                                           |
|  Manatsawin Hanmongkolchai  04/11/2023    |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PalitGPUController.h"

class RGBController_PalitGPU : public RGBController
{
public:
    RGBController_PalitGPU(PalitGPUController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PalitGPUController* controller;
};
