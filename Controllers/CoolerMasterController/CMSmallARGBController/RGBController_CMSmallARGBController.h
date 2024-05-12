/*---------------------------------------------------------*\
| RGBController_CMSmallARGBController.h                     |
|                                                           |
|   RGBController for Cooler Master Small ARGB controller   |
|                                                           |
|   Chris M (Dr_No)                             31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMSmallARGBController.h"

#define CM_SMALL_ARGB_MIN_LEDS          4
#define CM_SMALL_ARGB_MAX_LEDS          48
#define CM_SMALL_ARGB_BRIGHTNESS_MAX    0xFF
#define CM_SMALL_ARGB_FW0012            "A202104052336"

class RGBController_CMSmallARGBController : public RGBController
{
public:
    RGBController_CMSmallARGBController(CMSmallARGBController* controller_ptr);
    ~RGBController_CMSmallARGBController();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    void        Init_Controller();
    int         GetDeviceMode();

    CMSmallARGBController*  controller;
};
