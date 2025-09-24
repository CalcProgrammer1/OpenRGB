/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion.h                         |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion SMBus       |
|   motherboard                                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusionController.h"

class RGBController_RGBFusion : public RGBController
{
public:
    RGBController_RGBFusion(RGBFusionController* controller_ptr);
    ~RGBController_RGBFusion();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RGBFusionController* controller;

    int         GetDeviceMode();
};
