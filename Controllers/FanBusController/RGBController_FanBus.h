/*-----------------------------------------*\
|  RGBController_FanBus.h                   |
|                                           |
|  Generic RGB Interface for FanBus devices |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/12/2021   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "FanBusController.h"

class RGBController_FanBus : public RGBController
{
public:
    RGBController_FanBus(FanBusController* controller_ptr);
    ~RGBController_FanBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    FanBusController*   controller;
};
