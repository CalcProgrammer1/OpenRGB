/*---------------------------------------------------------*\
| RGBController_PhilipsWiz.h                                |
|                                                           |
|   RGBController for Philips Wiz                           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      03 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PhilipsWizController.h"

class RGBController_PhilipsWiz : public RGBController
{
public:
    RGBController_PhilipsWiz(PhilipsWizController* controller_ptr);
    ~RGBController_PhilipsWiz();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PhilipsWizController* controller;
};
