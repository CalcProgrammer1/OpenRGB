/*------------------------------------------*\
|  RGBController_SteelSeriesSensei.h         |
|                                            |
|  Generic RGB Interface SteelSeriesSensei   |
|  Class                                     |
|  Based on Rival controller by              |
|  B Horn (bahorn) 13/05/2020                |
\*------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesSenseiController.h"

class RGBController_SteelSeriesSensei : public RGBController
{
public:
    RGBController_SteelSeriesSensei(SteelSeriesSenseiController* controller_ptr);
    ~RGBController_SteelSeriesSensei();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesSenseiController* controller;
};
