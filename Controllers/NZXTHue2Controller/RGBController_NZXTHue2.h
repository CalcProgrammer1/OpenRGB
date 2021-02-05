/*-----------------------------------------*\
|  RGBController_NZXTHue2.h                 |
|                                           |
|  Generic RGB Interface for NZXT Hue 2     |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/29/2019  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "NZXTHue2Controller.h"

class RGBController_NZXTHue2 : public RGBController
{
public:
    RGBController_NZXTHue2(NZXTHue2Controller* hue2_ptr);
    ~RGBController_NZXTHue2();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    NZXTHue2Controller*         hue2;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
