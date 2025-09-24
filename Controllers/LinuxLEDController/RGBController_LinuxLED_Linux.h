/*---------------------------------------------------------*\
| RGBController_LinuxLED.h                                  |
|                                                           |
|   RGBController for Linux sysfs LEDs                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LinuxLEDController* controller;
};
