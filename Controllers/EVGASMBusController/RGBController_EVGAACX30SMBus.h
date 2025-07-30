/*---------------------------------------------------------*\
| RGBController_EVGAACX30SMBus.h                            |
|                                                           |
|   RGBController for SMBus EVGA ACX 30 motherboards        |
|                                                           |
|   Balázs Triszka (balika011)                  21 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAACX30SMBusController.h"

class RGBController_EVGAACX30SMBus : public RGBController
{
public:
    RGBController_EVGAACX30SMBus(EVGAACX30SMBusController* controller_ptr);
    ~RGBController_EVGAACX30SMBus();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EVGAACX30SMBusController *controller;
};
