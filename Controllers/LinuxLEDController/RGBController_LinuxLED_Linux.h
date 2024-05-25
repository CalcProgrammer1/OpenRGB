/*---------------------------------------------------------*\
| RGBController_LinuxLED.h                                  |
|                                                           |
|   RGBController for Linux sysfs LEDs                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LinuxLEDController_Linux.h"

class RGBController_LinuxLED : public RGBController
{
public:
    RGBController_LinuxLED(LinuxLEDController* controller_ptr);
    ~RGBController_LinuxLED();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LinuxLEDController* controller;
};
