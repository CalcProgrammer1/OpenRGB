/*---------------------------------------------------------*\
| RGBController_CMARGBController.h                          |
|                                                           |
|   RGBController for Cooler Master ARGB controller         |
|                                                           |
|   Chris M (Dr_No)                             14 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMARGBController.h"

#define first_zone(zn)          ((zones.size() > 1) ? 1 : 0)
#define last_zone(zn)           ((zones.size() > 1) ? 4 : 1)

class RGBController_CMARGBController : public RGBController
{
public:
    RGBController_CMARGBController(CMARGBController* controller_ptr);
    ~RGBController_CMARGBController();

    void        SetupZones();
    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    void        Init_Controller();
    int         GetDeviceMode();
    int         GetLED_Zone(int led_idx);

    CMARGBController*   controller;
};
