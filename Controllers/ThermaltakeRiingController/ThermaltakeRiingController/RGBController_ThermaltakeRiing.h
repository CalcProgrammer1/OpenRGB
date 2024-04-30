/*-----------------------------------------*\
|  RGBController_ThermaltakeRiing.h         |
|                                           |
|  Generic RGB Interface for Thermaltake    |
|  Riing controller                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/9/2020    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ThermaltakeRiingController.h"

class RGBController_ThermaltakeRiing : public RGBController
{
public:
    RGBController_ThermaltakeRiing(ThermaltakeRiingController* controller_ptr);
    ~RGBController_ThermaltakeRiing();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ThermaltakeRiingController* controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
