/*---------------------------------------------------------*\
| RGBController_LenovoMotherboard.h                         |
|                                                           |
|   RGBController for Lenovo motherboard                    |
|                                                           |
|   Morgan Guimard (morg)                       26 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LenovoMotherboardController* controller;
};
