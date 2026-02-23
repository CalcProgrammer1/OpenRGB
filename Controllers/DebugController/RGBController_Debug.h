/*---------------------------------------------------------*\
| RGBController_Debug.h                                     |
|                                                           |
|   Debug RGBController that can mimic various devices for  |
|   development and test purposes                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <nlohmann/json.hpp>
#include "RGBController.h"

using json = nlohmann::json;

class RGBController_Debug : public RGBController
{
public:
    RGBController_Debug(bool custom, json settings);
    ~RGBController_Debug();

    void        SetupZones();

    void        DeviceConfigureZone(int zone_idx);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    json        debug_settings;
    bool        custom_controller;
};
