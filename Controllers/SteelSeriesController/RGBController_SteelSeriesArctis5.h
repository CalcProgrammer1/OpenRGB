/*-----------------------------------------*\
|  RGBController_SteelSeriesArctis5.h       |
|                                           |
|  Generic RGB Interface SteelSeriesArctis5 |
|  Class                                    |
|                                           |
|  Morgan Guimard 3/04/2022                 |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesArctis5Controller.h"

class RGBController_SteelSeriesArctis5 : public RGBController
{
public:
    RGBController_SteelSeriesArctis5(SteelSeriesArctis5Controller* controller_ptr);
    ~RGBController_SteelSeriesArctis5();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    SteelSeriesArctis5Controller* controller;
};
