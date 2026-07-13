/*---------------------------------------------------------*\
| RGBController_SkydimoHID.h                                |
|                                                           |
|   RGBController for Skydimo HID devices                   |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  09 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SkydimoHIDController.h"

class RGBController_SkydimoHID : public RGBController
{
public:
    RGBController_SkydimoHID(SkydimoHIDController* controller_ptr);
    ~RGBController_SkydimoHID();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    SkydimoHIDController* controller;
};
