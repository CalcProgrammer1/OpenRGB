/*---------------------------------------------------------*\
| RGBController_LogitechG910.h                              |
|                                                           |
|   RGBController for Logitech G910 Orion Spectrum          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechG910Controller.h"

class RGBController_LogitechG910 : public RGBController
{
public:
    RGBController_LogitechG910(LogitechG910Controller* controller_ptr);
    ~RGBController_LogitechG910();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG910Controller* controller;
};
