/*-----------------------------------------*\
|  RGBController_MSIMysticLight.h           |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  MSI Mystic Light USB Driver              |
|                                           |
|  T-bond 3/4/2020                          |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "MSIMysticLightController.h"
#include <vector>

class RGBController_MSIMysticLight: public RGBController
{
public:
    RGBController_MSIMysticLight(MSIMysticLightController* controller_ptr);
    ~RGBController_MSIMysticLight();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    void        SetupModes();
    void        UpdateLed(int zone, int led);
    MSI_ZONE    ZoneFromPos(int zone);
    void        SetupMode(const char *name, MSI_MODE mode, unsigned int flags);

    MSIMysticLightController*       controller;
};
