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
#include <map>
#include <vector>

struct LedPort
{
    const char* name;
    int header;
};

typedef std::vector< std::vector<LedPort> > ZoneLeds;

class RGBController_MSIMysticLight: public RGBController
{
public:
    RGBController_MSIMysticLight(MSIMysticLightController* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    void        SetupModes();
    void        UpdateLed(int zone, int led);
    ZONE        ZoneFromPos(int zone);
    void        SetupMode(const char *name, EFFECT mode, unsigned int flags);

    MSIMysticLightController*       controller;
};
