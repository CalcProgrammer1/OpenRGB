/*---------------------------------------------------------*\
| RGBController_EVGAMouse.h                                 |
|                                                           |
|   RGBController for EVGA mouse                            |
|                                                           |
|   Cooper Knaak                                23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "EVGAMouseController.h"

#define EVGA_PERIPHERAL_BRIGHTNESS_MIN 0
#define EVGA_PERIPHERAL_BRIGHTNESS_MAX 100
#define EVGA_PERIPHERAL_SPEED_SLOWEST 0
#define EVGA_PERIPHERAL_SPEED_FASTEST 100

class RGBController_EVGAMouse : public RGBController
{
public:
    RGBController_EVGAMouse(EVGAMouseController* evga);
    ~RGBController_EVGAMouse();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    void        Init_Controller();
    int         GetDeviceMode();

    EVGAMouseController* controller;
};
