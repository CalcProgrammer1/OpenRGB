/*-----------------------------------------*\
|  RGBController_ZETEdgeAirPro.h            |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ZET Edge Air Pro RGB USB Driver          |
|                                           |
|  Guimard Morgan (morg) 1/29/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZETEdgeAirProController.h"

class RGBController_ZETEdgeAirPro : public RGBController
{
public:
    RGBController_ZETEdgeAirPro(ZETEdgeAirProController* controller_ptr);
    ~RGBController_ZETEdgeAirPro();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ZETEdgeAirProController* controller;
};
