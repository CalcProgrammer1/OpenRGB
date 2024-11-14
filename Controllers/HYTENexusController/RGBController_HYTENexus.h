/*---------------------------------------------------------*\
| RGBController_HYTENexus.h                                 |
|                                                           |
|   RGBController for HYTE Nexus                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "HYTENexusController.h"
#include "RGBController.h"

class RGBController_HYTENexus : public RGBController
{
public:
    RGBController_HYTENexus(HYTENexusController* controller_ptr);
    ~RGBController_HYTENexus();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HYTENexusController* controller;
};
