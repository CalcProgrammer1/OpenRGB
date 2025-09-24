/*---------------------------------------------------------*\
| RGBController_LenovoK510.h                                |
|                                                           |
|   RGBController for Lenovo Legion K510 keyboard           |
|                                                           |
|   Bnyro                                       27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LenovoK510Controller.h"

class RGBController_LenovoK510 : public RGBController
{
public:
    RGBController_LenovoK510(LenovoK510Controller* controller_ptr);
    ~RGBController_LenovoK510();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LenovoK510Controller* controller;

    void ReadAndUpdateCurrentDeviceState();
};
