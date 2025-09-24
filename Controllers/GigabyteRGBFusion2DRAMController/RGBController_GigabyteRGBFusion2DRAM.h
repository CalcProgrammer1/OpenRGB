/*---------------------------------------------------------*\
| GigabyteRGBController_RGBFusion2DRAM.h                    |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 RAM       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2DRAMController.h"

class RGBController_RGBFusion2DRAM : public RGBController
{
public:
    RGBController_RGBFusion2DRAM(RGBFusion2DRAMController* controller_ptr);
    ~RGBController_RGBFusion2DRAM();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RGBFusion2DRAMController* controller;
};
