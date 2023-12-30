/*-----------------------------------------*\
|  RGBController_RoccatBurstProAir.h        |
|                                           |
|  Generic RGB Interface for Roccat Burst   |
|  Pro Air Mouse controller                 |
|                                           |
|  Morgan Guimard (morg)     6/16/2022      |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatBurstProAirController.h"

class RGBController_RoccatBurstProAir : public RGBController
{
public:
    RGBController_RoccatBurstProAir(RoccatBurstProAirController* controller_ptr);
    ~RGBController_RoccatBurstProAir();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    RoccatBurstProAirController* controller;
};
