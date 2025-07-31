/*---------------------------------------------------------*\
| RGBController_Debug.h                                     |
|                                                           |
|   Debug RGBController that can mimic various devices for  |
|   development and test purposes                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <nlohmann/json.hpp>
#include "RGBController.h"

using json = nlohmann::json;

class RGBController_Debug : public RGBController
{
public:
    RGBController_Debug(bool custom_controller, json debug_settings);
    ~RGBController_Debug();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
};
