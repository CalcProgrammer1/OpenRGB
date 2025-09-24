/*---------------------------------------------------------*\
|  RGBController_KingstonFuryDRAM.h                         |
|                                                           |
|   Driver for Kingston Fury DDR4/5 RAM modules             |
|                                                           |
|   Geofrey Mon (geofbot)                       14 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "KingstonFuryDRAMController.h"

class RGBController_KingstonFuryDRAM : public RGBController
{
public:
    RGBController_KingstonFuryDRAM(KingstonFuryDRAMController* controller_ptr);
    ~RGBController_KingstonFuryDRAM();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    unsigned char               GetRealModeValue();
    KingstonFuryDRAMController* controller;
};
