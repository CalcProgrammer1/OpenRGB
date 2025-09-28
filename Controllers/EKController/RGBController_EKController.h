/*---------------------------------------------------------*\
| RGBController_EKController.h                              |
|                                                           |
|   RGBController for EK Loop Connect                       |
|                                                           |
|   Chris M (Dr_No)                             16 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EKController.h"

class RGBController_EKController : public RGBController
{
public:
    RGBController_EKController(EKController* controller_ptr);
    ~RGBController_EKController();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EKController* controller;
};
