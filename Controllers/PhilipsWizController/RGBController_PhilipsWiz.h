/*---------------------------------------------------------*\
| RGBController_PhilipsWiz.h                                |
|                                                           |
|   RGBController for Philips Wiz                           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      03 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PhilipsWizController* controller;
};
