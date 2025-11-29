/*---------------------------------------------------------*\
| RGBController_LaviewTechnology.h                          |
|                                                           |
|   RGBController for Laview Tech. mice, including Glorious |
|                                                           |
|   Kosta A (kostaarvanitis)                    27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LaviewTechnologyController.h"

class RGBController_LaviewTechnology : public RGBController
{
public:
    RGBController_LaviewTechnology(LaviewTechnologyController* controller_ptr);
    ~RGBController_LaviewTechnology();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LaviewTechnologyController* controller;
};
