/*---------------------------------------------------------*\
| RGBController_ValveSteamMachine_Linux.h                   |
|                                                           |
|   RGBController for Valve Steam Machine LEDs              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#ifdef __linux__
#include "ValveSteamMachineController_Linux.h"
#else
#include "ValveSteamMachineController_Windows.h"
#endif

class RGBController_ValveSteamMachine : public RGBController
{
public:
    RGBController_ValveSteamMachine(ValveSteamMachineController* controller_ptr);
    ~RGBController_ValveSteamMachine();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ValveSteamMachineController*    controller;
    std::vector<std::string>        effects;
};