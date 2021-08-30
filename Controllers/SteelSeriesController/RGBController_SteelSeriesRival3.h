/*-----------------------------------------*\
|  RGBController_SteelSeriesRival.h         |
|                                           |
|  Generic RGB Interface SteelSeriesRival3  |
|  Class                                    |
|                                           |
|  B Horn (bahorn) 29/08/2021               |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesRival3Controller.h"

class RGBController_SteelSeriesRival3 : public RGBController
{
public:
    RGBController_SteelSeriesRival3(SteelSeriesRival3Controller* rival_ptr);
    ~RGBController_SteelSeriesRival3();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
    void        DeviceSaveMode();


private:
    SteelSeriesRival3Controller*     rival;
};
