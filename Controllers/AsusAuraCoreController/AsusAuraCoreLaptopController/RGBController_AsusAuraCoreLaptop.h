/*---------------------------------------------------------*\
| RGBController_AsusAuraCoreLaptop.h                        |
|                                                           |
|   RGBController for ASUS ROG Aura Core Laptop             |
|                                                           |
|   Chris M (Dr_No)                             28 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <algorithm>
#include <vector>
#include "RGBController.h"
#include "AsusAuraCoreLaptopController.h"

class RGBController_AsusAuraCoreLaptop : public RGBController
{
public:
    RGBController_AsusAuraCoreLaptop(AsusAuraCoreLaptopController* controller_ptr);
    ~RGBController_AsusAuraCoreLaptop();

    void                SetupZones();

    void                DeviceUpdateLEDs();
    void                DeviceUpdateZoneLEDs(int zone);
    void                DeviceUpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    RGBColor                            null_color              = 0;
    std::vector<RGBColor *>             buffer_map;

    AsusAuraCoreLaptopController*       controller;
};
