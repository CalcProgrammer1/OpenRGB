/*-----------------------------------*\
|  RGBController_LenovoM300.h         |
|                                     |
|  Header file for Lenovo M300 Mouse  |
|  RGB Controller                     |
|                                     |
|  Wayne Riordan          9 Jan 2024  |
\*-----------------------------------*/

#pragma once

#include "RGBController.h"
#include "LenovoM300Controller.h"

class RGBController_LenovoM300 : public RGBController
{
public:
    RGBController_LenovoM300(LenovoM300Controller* controller_ptr);
    ~RGBController_LenovoM300();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LenovoM300Controller* controller;
};
