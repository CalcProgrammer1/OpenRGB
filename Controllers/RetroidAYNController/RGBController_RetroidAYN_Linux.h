/*---------------------------------------------------------*\
| RGBController_RetroidAYN_Linux.h                          |
|                                                           |
|   RGBController for Retroid and AYN handheld RGB LEDs     |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      08 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RetroidAYNController_Linux.h"

class RGBController_RetroidAYN : public RGBController
{
public:
    RGBController_RetroidAYN(RetroidAYNController* controller_ptr);
    ~RGBController_RetroidAYN();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RetroidAYNController*   controller;
};
