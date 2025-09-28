/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2SMBus.h                   |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 SMBus     |
|   motherboard                                             |
|                                                           |
|   Matt Harper                                 05 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2SMBusController.h"

class RGBController_RGBFusion2SMBus : public RGBController
{
public:
    RGBController_RGBFusion2SMBus(RGBFusion2SMBusController* controller_ptr);
    ~RGBController_RGBFusion2SMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RGBFusion2SMBusController* controller;

    int         GetDeviceMode();
};
