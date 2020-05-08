
/*-----------------------------------------*\
|  RGBController_RGBFusion2SMBus.h          |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2 SM BUs Driver      |
|                                           |
|  Matt Harper	(5/5/2020)		    |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RGBFusion2SMBusController.h"

class RGBController_RGBFusion2SMBus : public RGBController
{
public:
    RGBController_RGBFusion2SMBus(RGBFusion2SMBusController* rgb_fusion_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    RGBFusion2SMBusController* rgb_fusion;

    int         GetDeviceMode();
};
