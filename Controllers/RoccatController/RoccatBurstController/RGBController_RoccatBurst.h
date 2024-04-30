/*-----------------------------------------*\
|  RGBController_RoccatBurst.h              |
|                                           |
|  Generic RGB Interface for Roccat Burst   |
|  Core and Pro Mouse controller             |
|                                           |
|  Morgan Guimard (morg)     6/01/2022      |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatBurstController.h"

class RGBController_RoccatBurst : public RGBController
{
public:
    RGBController_RoccatBurst(RoccatBurstController* controller_ptr, unsigned int leds_count);
    ~RGBController_RoccatBurst();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatBurstController* controller;
    unsigned int leds_count;
};
