/*---------------------------------------------------------*\
| RGBController_RoccatKoneXTD.h                             |
|                                                           |
|   RGBController for Roccat Kone XTD                       |
|                                                           |
|   Julian                                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatKoneXTDController.h"

class RGBController_RoccatKoneXTD : public RGBController
{
public:
    RGBController_RoccatKoneXTD(RoccatKoneXTDController* controller_ptr);
    ~RGBController_RoccatKoneXTD();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatKoneXTDController*    controller;
    bool                        direct_active;
};
