/*-------------------------------------------*\
|  RGBController_GMOW.h                       |
|                                             |
|  Support for the Glorious Model O Wireless  |
|                                             |
|  Matt Silva (thesilvanator) 05/2022         |
\*-------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SinowealthGMOWController.h"

class RGBController_GMOW : public RGBController
{
public:
    RGBController_GMOW(SinowealthGMOWController* sinowealth_ptr);
    ~RGBController_GMOW();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthGMOWController* controller;
};
