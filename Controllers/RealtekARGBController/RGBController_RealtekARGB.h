/*---------------------------------------------------------*\
| RGBController_RealtekARGB.h                               |
|                                                           |
|   RGBController for Realtek USB ARGB ICs                  |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RealtekARGBController.h"

class RGBController_RealtekARGB : public RGBController
{
public:
    RGBController_RealtekARGB(RealtekARGBController* controller_ptr);
    ~RGBController_RealtekARGB();

    void        SetupModes();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int zone);
    void        DeviceUpdateMode();

private:
    RealtekARGBController* controller;
    std::vector<int> valid_grp;
    bool ready_to_reboot[REALTEK_ARGB_NUM_ARGB_GRP] = {false};
};
