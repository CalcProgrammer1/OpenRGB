/*-----------------------------------------*\
|  RGBController_Yeelight.h                 |
|                                           |
|  Generic RGB Interface for Yeelight       |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2021   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "YeelightController.h"

class RGBController_Yeelight : public RGBController
{
public:
    RGBController_Yeelight(YeelightController* light_ptr);
    ~RGBController_Yeelight();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    YeelightController* light;
};
