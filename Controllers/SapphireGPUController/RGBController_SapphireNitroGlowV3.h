/*-----------------------------------------*\
|  RGBController_SapphireNitroGlowV3.h      |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Sapphire Nitro Glow V3 GPU RGB Driver    |
|                                           |
|  K900 2/3/2021                            |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SapphireNitroGlowV3Controller.h"

class RGBController_SapphireNitroGlowV3 : public RGBController
{
public:
    RGBController_SapphireNitroGlowV3(SapphireNitroGlowV3Controller* sapphire_ptr);
    ~RGBController_SapphireNitroGlowV3();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    SapphireNitroGlowV3Controller* sapphire;

    void        ReadConfiguration();
};
