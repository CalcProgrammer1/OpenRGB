/*---------------------------------------------------------*\
| RGBController_CMRGBController.h                           |
|                                                           |
|   RGBController for Cooler Master RGB controller          |
|                                                           |
|   Nic W (midgetspy)                           13 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMRGBController.h"

class RGBController_CMRGBController : public RGBController
{
public:
    RGBController_CMRGBController(CMRGBController* controller_ptr);
    ~RGBController_CMRGBController();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CMRGBController*    controller;
    void                LoadConfigFromDeviceController(int device_mode);
    void                ReadAllModeConfigsFromDevice();
};
