/*---------------------------------------------------------*\
| RGBController_SkydimoSerial.h                             |
|                                                           |
|   RGBController for Skydimo serial devices                |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  30 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SkydimoSerialController.h"

class RGBController_SkydimoSerial : public RGBController
{
public:
    RGBController_SkydimoSerial(SkydimoSerialController* controller_ptr);
    ~RGBController_SkydimoSerial();

    void        SetupZones();
    void        DeviceConfigureZone(int zone_idx);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    SkydimoSerialController* controller;
};
