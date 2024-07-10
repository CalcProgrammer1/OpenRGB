/*-----------------------------------------*\
|  RGBController_SteelSeriesRival.h         |
|                                           |
|  Generic RGB Interface SteelSeriesRival   |
|  Class                                    |
|                                           |
|  B Horn (bahorn) 13/05/2020               |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesRivalController.h"

class RGBController_SteelSeriesRival : public RGBController
{
public:
    RGBController_SteelSeriesRival(SteelSeriesRivalController* controller_ptr);
    ~RGBController_SteelSeriesRival();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    SteelSeriesRivalController* controller;
};
