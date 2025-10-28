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
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LenovoK510Controller* controller;

    void ReadAndUpdateCurrentDeviceState();
};
