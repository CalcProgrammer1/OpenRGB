/*-----------------------------------------*\
|  RGBController_PNYLovelaceGPU.h           |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  PNY GPU RGB  (Lovelace) Driver           |
|                                           |
|  yufan   10/1/2023                        |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PNYLovelaceGPUController.h"

class RGBController_PNYLovelaceGPU : public RGBController
{
public:
    RGBController_PNYLovelaceGPU(PNYLovelaceGPUController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PNYLovelaceGPUController* controller;
};
