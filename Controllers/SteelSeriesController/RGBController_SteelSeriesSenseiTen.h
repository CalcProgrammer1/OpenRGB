/*------------------------------------------*\
|  RGBController_SteelSeriesSenseiTen.h      |
|                                            |
|  Generic RGB Interface SteelSeriesSenseiTen|
|  Class                                     |
|  Based on Rival controller by              |
|  B Horn (bahorn) 13/05/2020                |
\*------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesSenseiTenController.h"

class RGBController_SteelSeriesSenseiTen : public RGBController
{
public:
    RGBController_SteelSeriesSenseiTen(SteelSeriesSenseiTenController* sensei_ptr);
    ~RGBController_SteelSeriesSenseiTen();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    SteelSeriesSenseiTenController*     sensei;
};
