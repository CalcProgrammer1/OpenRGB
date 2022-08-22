/*-----------------------------------------*\
|  RGBController_TrustGXT180.h              |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Trust GXT 180 USB Driver                 |
|                                           |
|  Guimard Morgan (morg) 3/24/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "TrustGXT180Controller.h"

class RGBController_TrustGXT180 : public RGBController
{
public:
    RGBController_TrustGXT180(TrustGXT180Controller* controller_ptr);
    ~RGBController_TrustGXT180();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    TrustGXT180Controller* controller;
};
