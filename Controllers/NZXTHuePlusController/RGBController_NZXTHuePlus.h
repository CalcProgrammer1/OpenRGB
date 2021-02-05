/*-----------------------------------------*\
|  RGBController_NZXTHuePlus.h              |
|                                           |
|  Generic RGB Interface for NZXT Hue+      |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "serial_port.h"
#include "NZXTHuePlusController.h"

class RGBController_HuePlus : public RGBController
{
public:
    RGBController_HuePlus(HuePlusController* hueplus_ptr);
    ~RGBController_HuePlus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    HuePlusController*          hueplus;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
