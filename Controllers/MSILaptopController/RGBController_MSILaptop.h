/*---------------------------------------------------------*\
| RGBController_MSILaptop.h                                 |
|                                                           |
|   RGBController for MSI laptop SteelSeries RGB devices    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSILaptopController.h"

class RGBController_MSILaptop : public RGBController
{
public:
    RGBController_MSILaptop(MSILaptopController* controller_ptr, const MSILaptopModel* model_ptr);
    ~RGBController_MSILaptop();

    void    SetupZones();

    void    ResizeZone(int zone, int new_size);

    void    DeviceUpdateLEDs();
    void    UpdateZoneLEDs(int zone);
    void    UpdateSingleLED(int led);

    void    DeviceUpdateMode();

private:
    MSILaptopController* controller;
    const MSILaptopModel* model;
};
