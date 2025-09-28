/*---------------------------------------------------------*\
| RGBController_AsusAuraUSB.h                               |
|                                                           |
|   RGBController for ASUS Aura USB device                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraUSBController.h"

#define AURA_ADDRESSABLE_MAX_LEDS       120

class RGBController_AuraUSB : public RGBController
{
public:
    RGBController_AuraUSB(AuraUSBController* controller_ptr);
    ~RGBController_AuraUSB();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

protected:
    AuraUSBController*          controller;

private:
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
    bool                        initializedMode;
};
