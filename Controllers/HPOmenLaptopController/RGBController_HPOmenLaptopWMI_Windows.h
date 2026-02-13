/*---------------------------------------------------------*\
| RGBController_HPOmenLaptopWMI_Windows.h                   |
|                                                           |
|   RGBController for HP Omen laptop                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "HPOmenLaptopController_Windows.h"
#include "RGBController.h"

class RGBController_HPOmenLaptopWMI_Windows : public RGBController
{
public:
    RGBController_HPOmenLaptopWMI_Windows(HPOmenLaptopController_Windows *controller);
    ~RGBController_HPOmenLaptopWMI_Windows();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    HPOmenLaptopController_Windows *controller;
};
