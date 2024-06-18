/*---------------------------------------------------------*\
| RGBController_SRGBmodsLEDControllerV1.h                   |
|                                                           |
|   RGBController for SRGBmods LED Controller V1            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SRGBmodsLEDControllerV1.h"

#define SRGBMODS_LED_CONTROLLER_V1_NUM_CHANNELS 1

class RGBController_SRGBmodsLEDControllerV1 : public RGBController
{
public:
    RGBController_SRGBmodsLEDControllerV1(SRGBmodsLEDControllerV1* controller_ptr);
    ~RGBController_SRGBmodsLEDControllerV1();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SRGBmodsLEDControllerV1*    controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
