/*---------------------------------------------------------*\
| RGBController_HYTENexus.h                                 |
|                                                           |
|   RGBController for HYTE Nexus                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HYTENexusController* controller;
};
