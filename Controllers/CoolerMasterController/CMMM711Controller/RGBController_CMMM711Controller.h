/*---------------------------------------------------------*\
| RGBController_CMMM711Controller.h                         |
|                                                           |
|   RGBController for Cooler Master M711 mouse              |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMMM711Controller.h"

#define CM_MM_ARGB_BRIGHTNESS_MIN           0x00
#define CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT   0xFF
#define CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM  0x7F

class RGBController_CMMM711Controller : public RGBController
{
public:
    RGBController_CMMM711Controller(CMMM711Controller* controller_ptr);
    ~RGBController_CMMM711Controller();

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

    CMMM711Controller*  controller;
};
