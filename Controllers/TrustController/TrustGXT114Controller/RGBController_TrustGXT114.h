/*-----------------------------------------*\
|  RGBController_TrustGXT114.h              |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Trust GXT 114 USB Driver                 |
|                                           |
|  Guimard Morgan (morg) 1/24/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "TrustGXT114Controller.h"

class RGBController_TrustGXT114 : public RGBController
{
public:
    RGBController_TrustGXT114(TrustGXT114Controller* controller_ptr);
    ~RGBController_TrustGXT114();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    TrustGXT114Controller* controller;
};
