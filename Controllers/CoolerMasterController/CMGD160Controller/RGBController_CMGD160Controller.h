/*---------------------------------------------------------*\
| RGBController_CMGD160Controller.h                         |
|                                                           |
|   RGBController for Cooler Master GD160 ARGB Gaming Desk  |
|                                                           |
|   Logan Phillips (Eclipse)                    16 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   Adapted from CMMonitor controller code                  |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CMGD160Controller.h"

class RGBController_CMGD160Controller : public RGBController
{
public:
    RGBController_CMGD160Controller(CMGD160Controller* controller_ptr);
    ~RGBController_CMGD160Controller();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CMGD160Controller*   controller;
};