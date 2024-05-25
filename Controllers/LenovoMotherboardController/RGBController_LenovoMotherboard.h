/*---------------------------------------------------------*\
| RGBController_LenovoMotherboard.h                         |
|                                                           |
|   RGBController for Lenovo motherboard                    |
|                                                           |
|   Morgan Guimard (morg)                       26 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LenovoMotherboardController.h"

class RGBController_LenovoMotherboard : public RGBController
{
public:
    RGBController_LenovoMotherboard(LenovoMotherboardController* controller_ptr);
    ~RGBController_LenovoMotherboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LenovoMotherboardController* controller;
};
