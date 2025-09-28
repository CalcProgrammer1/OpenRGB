/*---------------------------------------------------------*\
| RGBController_LogitechG810.h                              |
|                                                           |
|   RGBController for Logitech G810 Orion Spectrum          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG810Controller.h"

class RGBController_LogitechG810 : public RGBController
{
public:
    RGBController_LogitechG810(LogitechG810Controller* controller_ptr);
    ~RGBController_LogitechG810();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG810Controller* controller;
};
