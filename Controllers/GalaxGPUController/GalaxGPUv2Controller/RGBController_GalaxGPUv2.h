/*---------------------------------------------------------*\
| RGBController_GalaxGPUv2.h                                |
|                                                           |
|   RGBController for Galax GPUs (Xtreme Tuner)             |
|                                                           |
|   Daniel Stuart  (daniel.stuart14)            26 may 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GalaxGPUv2Controller.h"

class RGBController_GalaxGPUv2 : public RGBController
{
public:
    RGBController_GalaxGPUv2(GalaxGPUv2Controller* controller_ptr);
    ~RGBController_GalaxGPUv2();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    GalaxGPUv2Controller* controller;

    int        GetDeviceMode();
};
