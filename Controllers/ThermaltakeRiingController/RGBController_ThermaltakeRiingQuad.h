/*-------------------------------------------------------------------*\
|  RGBController_ThermaltakeRiingQuad.cpp                             |
|                                                                     |
|  Driver for Thermaltake Riing Quad Controller                       |
|                                                                     |
|  Chris M (Dr_No)          15th Feb 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ThermaltakeRiingQuadController.h"

class RGBController_ThermaltakeRiingQuad : public RGBController
{
public:
    RGBController_ThermaltakeRiingQuad(ThermaltakeRiingQuadController* quad_ptr);
    ~RGBController_ThermaltakeRiingQuad();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    ThermaltakeRiingQuadController* quad;
    std::vector<unsigned int>       leds_channel;
    std::vector<unsigned int>       zones_channel;
};
