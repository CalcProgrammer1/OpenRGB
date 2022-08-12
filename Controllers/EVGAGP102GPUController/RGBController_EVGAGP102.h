/*-----------------------------------------*\
|  RGBController_EVGAGP102.h                |
|                                           |
|  EVGA Generic RGB Interface for OpenRGB   |
|  EVGA GP102-based Nvidia GPUs.            |
|                                           |
|  Fabricio Murta (avengerx) 1/31/2021      |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGP102Controller.h"

class RGBController_EVGAGP102 : public RGBController
{
public:
    RGBController_EVGAGP102(std::vector<EVGAGP102Controller*> evga_ptr);
    ~RGBController_EVGAGP102();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    std::vector<EVGAGP102Controller*> evga;
};
