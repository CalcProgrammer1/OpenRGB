/*---------------------------------------------------------*\
| RGBController_RaspberryPi_Linux.h                         |
|                                                           |
|   RGBController for Raspberry Pi GPIO ARGB                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RaspberryPiController_Linux.h"
#include "RGBController.h"

class RGBController_RaspberryPi : public RGBController
{
public:
    RGBController_RaspberryPi(RaspberryPiController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RaspberryPiController*      controller;
};
