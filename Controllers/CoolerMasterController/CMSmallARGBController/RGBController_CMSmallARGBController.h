/*---------------------------------------------------------*\
| RGBController_CMSmallARGBController.h                     |
|                                                           |
|   RGBController for Cooler Master Small ARGB controller   |
|                                                           |
|   Chris M (Dr_No)                             31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMSmallARGBController.h"

#define CM_SMALL_ARGB_MIN_LEDS          4
#define CM_SMALL_ARGB_MAX_LEDS          48
#define CM_SMALL_ARGB_BRIGHTNESS_MAX    0xFF

class RGBController_CMSmallARGBController : public RGBController
{
public:
    RGBController_CMSmallARGBController(CMSmallARGBController* controller_ptr);
    ~RGBController_CMSmallARGBController();

    void        SetupModes();
    void        SetupZones();

    void        DeviceConfigureZone(int zone_idx);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CMSmallARGBController*  controller;
};
