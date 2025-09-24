/*---------------------------------------------------------*\
| RGBController_QMKKeychron.h                               |
|                                                           |
|   RGBController for Keychron QMK-based keyboards          |
|                                                           |
|   Amadej Kastelic                             21 Jun 2026 |
|   Adam Honse <calcprogrammer1@gmail.com>      22 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "QMKKeychronController.h"

class RGBController_QMKKeychron : public RGBController
{
public:
    RGBController_QMKKeychron(QMKKeychronController* controller_ptr);
    ~RGBController_QMKKeychron();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    QMKKeychronController* controller;
};
