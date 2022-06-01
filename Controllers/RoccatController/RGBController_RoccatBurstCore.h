/*-----------------------------------------*\
|  RGBController_RoccatBurstCore.h          |
|                                           |
|  Generic RGB Interface for Roccat Burst   |
|  Core Mouse controller                    |
|                                           |
|  Morgan Guimard (morg)     6/01/2022      |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatBurstCoreController.h"

class RGBController_RoccatBurstCore : public RGBController
{
public:
    RGBController_RoccatBurstCore(RoccatBurstCoreController* controller_ptr);
    ~RGBController_RoccatBurstCore();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RoccatBurstCoreController* controller;
};
