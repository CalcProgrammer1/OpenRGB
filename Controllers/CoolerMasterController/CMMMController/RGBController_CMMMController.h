/*---------------------------------------------------------*\
| RGBController_CMMMController.h                            |
|                                                           |
|   RGBController for Cooler Master mouse                   |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Dracrius                                    12 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMMMController.h"

#define CM_MM_ARGB_BRIGHTNESS_MIN           0x00
#define CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT   0xFF
#define CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM  0x7F

class RGBController_CMMMController : public RGBController
{
public:
    RGBController_CMMMController(CMMMController* controller_ptr);
    ~RGBController_CMMMController();

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

    int         leds_count;

    CMMMController*  controller;
};
