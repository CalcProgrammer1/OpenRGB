/*---------------------------------------------------------*\
| RGBController_DDP.h                                       |
|                                                           |
|   RGBController for DDP devices                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        SetKeepaliveTime(unsigned int time_ms);

private:
    std::vector<DDPDevice>      devices;
    DDPController*              controller;
};
