/*---------------------------------------------------------*\
| RGBController_QMKVialRGB.h                                |
|                                                           |
|   RGBController for VialRGB QMK Keyboard Protocol         |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com)      29 Sep 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "QMKVialRGBController.h"
#include "RGBController.h"

class RGBController_QMKVialRGB : public RGBController
{
public:
    RGBController_QMKVialRGB(QMKVialRGBController* controller_ptr);
    ~RGBController_QMKVialRGB();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    QMKVialRGBController* controller;
};
