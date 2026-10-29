/*---------------------------------------------------------*\
| RGBController_FrameworkDesktop_Linux.h                    |
|                                                           |
|   RGBController for the Framework Desktop ARGB header     |
|                                                           |
|   Francis De Brabandere (francisdb@gmail.com) 16 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "FrameworkDesktopController_Linux.h"

class RGBController_FrameworkDesktop : public RGBController
{
public:
    RGBController_FrameworkDesktop(FrameworkDesktopController* controller_ptr);
    ~RGBController_FrameworkDesktop();

    void        SetupZones();

    void        DeviceConfigureZone(int zone_idx);

    void        DeviceUpdateLEDs(std::vector<RGBColor>& update_colors);
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    FrameworkDesktopController* controller;
};
