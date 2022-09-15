/*------------------------------------------*\
|  RGBController_GigabyteSuperIORGB.h               |
|                                            |
|  Generic RGB Interface for Gigabyte RGB    |
|                                            |
|  Ryan Frankcombe (422gRdHuX5uk) 9/11/2022  |
\*------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "GigabyteSuperIORGBController.h"

class RGBController_GigabyteSuperIORGB : public RGBController
{
public:
    RGBController_GigabyteSuperIORGB(GigabyteSuperIORGBController* controller_ptr);
    ~RGBController_GigabyteSuperIORGB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GigabyteSuperIORGBController*       controller;
};
