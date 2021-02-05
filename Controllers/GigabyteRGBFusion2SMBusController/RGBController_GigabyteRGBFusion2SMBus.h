
/*-----------------------------------------*\
|  RGBController_GigabyteRGBFusion2SMBus.h  |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2 SMBUS Driver       |
|                                           |
|  Matt Harper	(5/5/2020)		            |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2SMBusController.h"

class RGBController_RGBFusion2SMBus : public RGBController
{
public:
    RGBController_RGBFusion2SMBus(RGBFusion2SMBusController* rgb_fusion_ptr);
    ~RGBController_RGBFusion2SMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RGBFusion2SMBusController* rgb_fusion;

    int         GetDeviceMode();
};
