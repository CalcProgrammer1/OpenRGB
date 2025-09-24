/*---------------------------------------------------------*\
| RGBController_DDP.h                                       |
|                                                           |
|   RGBController for DDP devices                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "DDPController.h"

class RGBController_DDP : public RGBController
{
public:
    RGBController_DDP(std::vector<DDPDevice> device_list);
    ~RGBController_DDP();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        SetKeepaliveTime(unsigned int time_ms);

private:
    std::vector<DDPDevice>      devices;
    DDPController*              controller;
};
