/*-----------------------------------------*\
|  RGBController_GloriousModelO.h           |
|                                           |
|  Definitions and types for Glorious       |
|  and other Mice                           |
|                                           |
|  Niels Westphal (crashniels) 20/5/2020    |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GloriousModelOController.h"

class RGBController_GloriousModelO : public RGBController
{
public:
    RGBController_GloriousModelO(GloriousModelOController* gmo_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    GloriousModelOController* gmo;
};
