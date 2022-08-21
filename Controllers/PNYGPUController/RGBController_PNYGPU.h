/*-----------------------------------------*\
|  RGBController_PNYGPU.h                   |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  PNY GPU RGB  (Turing) Driver             |
|                                           |
|  KendallMorgan   10/17/2021               |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PNYGPUController.h"

class RGBController_PNYGPU : public RGBController
{
public:
    RGBController_PNYGPU(PNYGPUController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PNYGPUController* controller;
};
