/*-----------------------------------------*\
|  RGBController_SRGBmodsPico.h             |
|                                           |
|  Generic RGB Interface for SRGBmods       |
|  Raspberry Pi Pico LED Controller         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/21/2022   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SRGBmodsPicoController.h"

#define SRGBMODS_PICO_NUM_CHANNELS  2

class RGBController_SRGBmodsPico : public RGBController
{
public:
    RGBController_SRGBmodsPico(SRGBmodsPicoController* controller_ptr);
    ~RGBController_SRGBmodsPico();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SRGBmodsPicoController*     controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
