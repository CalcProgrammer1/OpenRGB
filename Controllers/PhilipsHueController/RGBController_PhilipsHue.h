/*-----------------------------------------*\
|  RGBController_PhilipsHue.h               |
|                                           |
|  Generic RGB Interface for Philips Hue    |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/15/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "PhilipsHueController.h"

class RGBController_PhilipsHue : public RGBController
{
public:
    RGBController_PhilipsHue(PhilipsHueController* light_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    PhilipsHueController* light;
};
