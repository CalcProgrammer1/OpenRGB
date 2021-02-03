/*-----------------------------------------*\
|  RGBController_SapphireNitroGlowV1.h      |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Sapphire Nitro Glow V1 GPU RGB Driver    |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/15/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SapphireNitroGlowV1Controller.h"

class RGBController_SapphireNitroGlowV1 : public RGBController
{
public:
    RGBController_SapphireNitroGlowV1(SapphireNitroGlowV1Controller* sapphire_ptr);
    ~RGBController_SapphireNitroGlowV1();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    SapphireNitroGlowV1Controller* sapphire;

    void        ReadConfiguration();
};
