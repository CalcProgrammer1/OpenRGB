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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GigabyteCastor3Controller*  controller;
};
