/*---------------------------------------------------------*\
| RGBController_EVGAX12.h                                   |
|                                                           |
|   RGBController for the EVGA X12 gaming mouse             |
|                                                           |
|   QuickMythril                               08 Aug 2026  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "EVGAX12Controller.h"
#include "RGBController.h"

class RGBController_EVGAX12 : public RGBController
{
public:
    RGBController_EVGAX12(EVGAX12Controller* controller_ptr);
    ~RGBController_EVGAX12();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);
    void DeviceUpdateMode();

private:
    void SetupZones();

    EVGAX12Controller* controller;
    unsigned int       direct_mode_index;
    unsigned int       off_mode_index;
};
