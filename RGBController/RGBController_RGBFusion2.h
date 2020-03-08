/*-----------------------------------------*\
|  RGBController_RGBFusion2.h               |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2.0 Driver           |
|                                           |
|  jackun 1/8/2020                          |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RGBFusion2Controller.h"
#include <map>
#include <vector>

struct LedPort
{
    const char* name;
    int header;
};

typedef std::vector< std::vector<LedPort> > ZoneLeds;
typedef std::map< std::string, ZoneLeds > KnownChannels;

class RGBController_RGBFusion2: public RGBController
{
public:
    RGBController_RGBFusion2(RGBFusion2Controller* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    RGBFusion2Controller*       controller;
    IT8297Report                report;
    unsigned int                per_strip_led_cnt;
};
