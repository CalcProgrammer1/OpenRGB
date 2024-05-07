/*---------------------------------------------------------*\
| RGBController_HIDLampArray.h                              |
|                                                           |
|   RGBController for HID LampArray Devices                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      26 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HIDLampArrayController.h"

class RGBController_HIDLampArray : public RGBController
{
public:
    RGBController_HIDLampArray(HIDLampArrayController* controller_ptr);
    ~RGBController_HIDLampArray();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HIDLampArrayController* controller;
};
