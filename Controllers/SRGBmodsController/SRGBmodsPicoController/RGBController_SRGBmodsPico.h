/*---------------------------------------------------------*\
| RGBController_SRGBmodsPico.h                              |
|                                                           |
|   RGBController for SRGBmods Raspberry Pi Pico LED        |
|   Controller                                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

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
    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SRGBmodsPicoController*     controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
