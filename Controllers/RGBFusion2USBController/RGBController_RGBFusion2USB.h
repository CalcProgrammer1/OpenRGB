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

#define RGBFusion2_Digital_LEDS_Min 0;
#define RGBFusion2_Digital_LEDS_Max 1024;
#define RGBFusion2_Digital_Direct_Offset (HDR_D_LED1_RGB - HDR_D_LED1);

struct LedPort
{
    const char* name;
    int header;
    int count;
};

typedef std::map< std::string, std::string > MBName;
typedef std::map< std::string, std::vector<LedPort> > ZoneLeds;
typedef std::map< std::string, ZoneLeds> KnownLayout;

class RGBController_RGBFusion2USB: public RGBController
{
public:
    RGBController_RGBFusion2USB(RGBFusion2USBController* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    void        Init_Controller();
    int         GetLED_Zone(int led_idx);

    RGBFusion2USBController*        controller;
    IT8297Report                    report;
    ZoneLeds                        layout;
};
