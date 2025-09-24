/*---------------------------------------------------------*\
| RGBController_SayoDevice.h                                |
|                                                           |
|   Controller for Sayo Devices                             |
|                                                           |
|   Richard Harris                              24 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SayoDeviceController.h"

class RGBController_SayoDevice : public RGBController
{
public:
    RGBController_SayoDevice(SayoDeviceController* controller_ptr);
    ~RGBController_SayoDevice();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    SayoDeviceController* controller;
};
