/*---------------------------------------------------------*\
| RGBController_LinuxRGBLED_Linux.h                         |
|                                                           |
|   RGBController for Linux RGB sysfs LEDs (multi_intensity)|
|                                                           |
|   This driver supports the Valve-style sysfs LED interface|
|   found at /sys/class/leds/valve-leds* where each LED     |
|   directory contains a multi_intensity file for RGB       |
|   control                                                 |
|                                                           |
|   All LEDs on a device are grouped into a single linear   |
|   zone.                                                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LinuxRGBLEDController_Linux.h"

class RGBController_LinuxRGBLED : public RGBController
{
public:
    RGBController_LinuxRGBLED(LinuxRGBLEDController* controller_ptr);
    ~RGBController_LinuxRGBLED();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LinuxRGBLEDController* controller;
};