/*---------------------------------------------------------*\
| RGBController_CMMM712Controller.h                         |
|                                                           |
|   RGBController for Cooler Master M712 mouse              |
|   Derived from RGBController_CMMM712Controller.h          |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Frans Meulenbroeks                          08 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CMMM712Controller.h"

#define CM_MM_ARGB_BRIGHTNESS_MIN            0x00
#define CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT    0xFF
#define CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM   0x7F

class RGBController_CMMM712Controller : public RGBController
{
public:
    RGBController_CMMM712Controller(CMMM712Controller* controller_ptr);
    ~RGBController_CMMM712Controller();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();
private:
    void        Init_Controller();

    CMMM712Controller*  controller;
};
