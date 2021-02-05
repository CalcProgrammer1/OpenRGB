/*-----------------------------------------*\
|  RGBController_AorusATC800.h              |
|                                           |
|  Generic RGB Interface for Aorus ATC 800  |
|  CPU Cooler                               |
|                                           |
|  Felipe Cavalcanti 08/13/2020             |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ATC800Controller.h"

class RGBController_AorusATC800 : public RGBController
{
public:
    RGBController_AorusATC800(ATC800Controller* logitech_ptr);
    ~RGBController_AorusATC800();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    ATC800Controller*   cooler;
};
