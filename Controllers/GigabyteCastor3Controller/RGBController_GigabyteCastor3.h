/*---------------------------------------------------------*\
|  RGBController_GigabyteCastor3.h                          |
|                                                           |
|  RGBController for Gigabyte Aorus Waterforce X II 360     |
|  AIO Cooler (Castor3 controller)                          |
|                                                           |
|  This file is part of the OpenRGB project                 |
|  SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteCastor3Controller.h"

class RGBController_GigabyteCastor3 : public RGBController
{
public:
    RGBController_GigabyteCastor3(GigabyteCastor3Controller* controller_ptr);
    ~RGBController_GigabyteCastor3();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GigabyteCastor3Controller*  controller;
};
