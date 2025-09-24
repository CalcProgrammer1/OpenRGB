/*---------------------------------------------------------*\
| RGBController_CorsairDRAM.h                               |
|                                                           |
|   RGBController for Corsair DRAM RGB controllers          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairDRAMController.h"

class RGBController_CorsairDRAM : public RGBController
{
public:
    RGBController_CorsairDRAM(CorsairDRAMController* controller_ptr);
    ~RGBController_CorsairDRAM();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairDRAMController* controller;
};
