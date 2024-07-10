/*-----------------------------------------*\
|  RGBController_SteelSeriesSiberia.h       |
|                                           |
|  Generic RGB Interface SteelSeriesSiberia |
|  Class                                    |
|                                           |
|  E Karlsson (pilophae) 18/06/2020         |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesSiberiaController.h"

class RGBController_SteelSeriesSiberia : public RGBController
{
public:
    RGBController_SteelSeriesSiberia(SteelSeriesSiberiaController* controller_ptr);
    ~RGBController_SteelSeriesSiberia();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesSiberiaController* controller;
};
