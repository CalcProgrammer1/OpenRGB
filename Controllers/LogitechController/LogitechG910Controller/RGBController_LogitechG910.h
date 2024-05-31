/*---------------------------------------------------------*\
| RGBController_LogitechG910.h                              |
|                                                           |
|   RGBController for Logitech G910 Orion Spectrum          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG910Controller* controller;
};
