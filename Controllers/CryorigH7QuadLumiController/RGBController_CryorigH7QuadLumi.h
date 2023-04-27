/*-----------------------------------------*\
|  RGBController_CryorigH7QuadLumi.h        |
|                                           |
|  Generic RGB Interface for Cryorig H7     |
|  Quad Lumi                                |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/15/2023   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CryorigH7QuadLumiController.h"

class RGBController_CryorigH7QuadLumi : public RGBController
{
public:
    RGBController_CryorigH7QuadLumi(CryorigH7QuadLumiController* controller_ptr);
    ~RGBController_CryorigH7QuadLumi();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CryorigH7QuadLumiController*    controller;
    std::vector<unsigned int>       leds_channel;
    std::vector<unsigned int>       zones_channel;
};
