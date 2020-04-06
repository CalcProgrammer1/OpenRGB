/*-----------------------------------------*\
|  RGBController_RGBFusion2USB.h            |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2.0 USB Driver       |
|                                           |
|  jackun 1/8/2020                          |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RGBFusion2USBController.h"
#include <map>
#include <vector>

struct LedPort
{
    const char* name;
    int header;
};

typedef std::vector< std::vector<LedPort> > ZoneLeds;
typedef std::map< std::string, ZoneLeds > KnownChannels;

class RGBController_RGBFusion2USB: public RGBController
{
public:
    RGBController_RGBFusion2USB(RGBFusion2USBController* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    RGBFusion2USBController*       controller;
    IT8297Report                   report;
};
